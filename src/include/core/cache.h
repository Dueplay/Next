#ifndef NEXT_CACHE_H
#define NEXT_CACHE_H

#include "core/utils.h"
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace Next {
static constexpr size_t DEFAULT_CACHE_CAPACITY = 10 * 1024 * 1024;
auto GetTimeUtc() noexcept -> uint64_t;

class Cache {
public:
  class CacheNode {
    friend class Cache;

  public:
    CacheNode() noexcept;
    CacheNode(std::string identifier, const std::vector<unsigned char> &data);
    void SetIdentifier(const std::string &identifier);
    void SetData(const std::vector<unsigned char> &data);
    void Serialize(std::vector<unsigned char> &destination);
    auto Size() const noexcept -> size_t;
    void UpdataTimeStamp() noexcept;
    auto GetTimeStamp() const noexcept -> uint64_t;

  private:
    std::string identifier_;
    std::vector<unsigned char> data_;
    uint64_t last_access_{0};
    CacheNode *prev_{nullptr};
    CacheNode *next_{nullptr};
  };

  explicit Cache(size_t capacity = DEFAULT_CACHE_CAPACITY) noexcept;
  NON_MOVE_AND_COPYABLE(Cache);

  auto GetOccupancy() const noexcept -> size_t;
  auto GetCapacity() const noexcept -> size_t;

  auto TryLoad(const std::string &resource_url,
               std::vector<unsigned char> &destination) -> bool;

  auto TryInsert(const std::string &resource_url,
                 const std::vector<unsigned char> &source) -> bool;

  void Clear();
  void EvictOneByUrl(const std::string& url) noexcept;
private:
  void EvictOne() noexcept;
  void RemoveFromList(const std::shared_ptr<CacheNode> &node) noexcept;
  void AppendToListTail(const std::shared_ptr<CacheNode> &node) noexcept;

  // shared_mutex to implement read/write lock
  std::shared_mutex mtx_;
  // use map to fast find ptr pos
  std::unordered_map<std::string, std::shared_ptr<CacheNode>> mapping_;
  const size_t capacity_{0};
  size_t occupancy_{0};
  const std::shared_ptr<CacheNode> header_;
  const std::shared_ptr<CacheNode> tailer_;
};

} // namespace Next
#endif