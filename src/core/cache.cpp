#include "core/cache.h"
#include <cassert>
#include <chrono>
#include <utility>

namespace Next {

auto GetTimeUtc() noexcept -> uint64_t {
  auto mill_since_epoch =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count();
  return mill_since_epoch;
}

Cache::CacheNode::CacheNode() noexcept { UpdataTimeStamp(); }
Cache::CacheNode::CacheNode(std::string identifier,
                            const std::vector<unsigned char> &data)
    : identifier_(std::move(identifier)), data_(data) {
  UpdataTimeStamp();
}
void Cache::CacheNode::SetIdentifier(const std::string &identifier) {
  identifier_ = identifier;
}
void Cache::CacheNode::SetData(const std::vector<unsigned char> &data) {
  data_ = data;
}
void Cache::CacheNode::Serialize(std::vector<unsigned char> &destination) {
  size_t resource_size = data_.size();
  size_t buffer_old_size = destination.size();
  destination.reserve(resource_size + buffer_old_size);
  destination.insert(destination.end(), data_.begin(), data_.end());
}
auto Cache::CacheNode::Size() const noexcept -> size_t { return data_.size(); }
void Cache::CacheNode::UpdataTimeStamp() noexcept {
  last_access_ = GetTimeUtc();
}
auto Cache::CacheNode::GetTimeStamp() const noexcept -> uint64_t {
  return last_access_;
}

Cache::Cache(size_t capacity) noexcept
    : capacity_(capacity), header_(std::make_unique<CacheNode>()),
      tailer_(std::make_unique<CacheNode>()) {
  header_->next_ = tailer_.get();
  tailer_->prev_ = header_.get();
}

auto Cache::GetOccupancy() const noexcept -> size_t { return occupancy_; }
auto Cache::GetCapacity() const noexcept -> size_t { return capacity_; }

auto Cache::TryLoad(const std::string &resource_url,
                    std::vector<unsigned char> &destination) -> bool {
  std::shared_lock<std::shared_mutex> lock(mtx_);
  auto iter = mapping_.find(resource_url);
  if (iter != mapping_.end()) {
    iter->second->Serialize(destination);
    RemoveFromList(iter->second);
    AppendToListTail(iter->second);
    iter->second->UpdataTimeStamp();
    return true;
  }
  return false;
}

auto Cache::TryInsert(const std::string &resource_url,
                      const std::vector<unsigned char> &source) -> bool {
  std::shared_lock<std::shared_mutex> lock(mtx_);
  auto iter = mapping_.find(resource_url);
  if (iter != mapping_.end()) {
    return false;
  }
  auto source_size = source.size();
  if (source_size > capacity_) {
    return false;
  }
  while (!mapping_.empty() && (capacity_ - occupancy_) < source_size) {
    EvictOne();
  }
  auto node = std::make_shared<CacheNode>(resource_url, source);
  AppendToListTail(node);
  occupancy_ += source_size;
  mapping_.emplace(resource_url, node);
  return true;
}

void Cache::Clear() {
  header_->next_ = tailer_.get();
  tailer_->prev_ = header_.get();
  mapping_.clear();
  occupancy_ = 0;
}

void Cache::EvictOne() noexcept {
  // lru策略，第一个是最久未使用的。
  auto *first_node = header_->next_;
  auto resource_size = first_node->Size();
  auto iter = mapping_.find(first_node->identifier_); // friend
  assert(iter != mapping_.end());
  RemoveFromList(iter->second);
  mapping_.erase(iter);
  occupancy_ -= resource_size;
}

void Cache::EvictOneByUrl(const std::string& url) noexcept {
  auto iter = mapping_.find(url); // friend
  assert(iter != mapping_.end());
  auto resource_size = iter->second->Size();
  RemoveFromList(iter->second);
  mapping_.erase(iter);
  occupancy_ -= resource_size;
}

void Cache::RemoveFromList(const std::shared_ptr<CacheNode> &node) noexcept {
  auto *node_ptr = node.get();
  auto *node_prev = node_ptr->prev_;
  auto *node_next = node_ptr->next_;
  node_prev->next_ = node_next;
  node_next->prev_ = node_prev;
}
void Cache::AppendToListTail(const std::shared_ptr<CacheNode> &node) noexcept {
  auto *node_ptr = node.get();
  auto *node_prev = tailer_->prev_;
  node_prev->next_ = node_ptr;
  tailer_->prev_ = node_ptr;
  node_ptr->prev_ = node_prev;
  node_ptr->next_ = tailer_.get();
}
} // namespace Next