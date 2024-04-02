/**
 * This is the unit test file for core/Cache class
 */

#include "core/cache.h"

#include <vector>
#include "http/http_utils.h"
#include "catch2/catch_test_macros.hpp"
#include <string>
#include <iostream>
#include "core/timer.h"
/* for convenience reason */
using Next::Cache;
using Next::Http::LoadFile;
using Next::Http::IsFileExists;
using Next::GetTimeUtc;
using Next::NowSinceEpoch;
using Next::NowSinceEpoch2;


TEST_CASE("[core/cache]") {
  const int capacity = 20;
  Cache cache(capacity);
  std::vector<unsigned char> data = {'h', 'e', 'l', 'l', 'o', '!'};
  const auto data_size = data.size();

  REQUIRE(cache.GetOccupancy() == 0);
  REQUIRE(cache.GetCapacity() == capacity);

  SECTION("cache should be able to cache data up to capacity, and start evict") {
    for (int i = 1; i <= capacity / data_size; i++) {
      std::string url = "url" + std::to_string(i);
      bool cache_success = cache.TryInsert(url, data);
      CHECK(cache_success);
      CHECK(cache.GetOccupancy() == i * data_size);
    }

    std::vector<unsigned char> read_buf;
    // all url1, url2, url3 should be available
    for (int i = 1; i <= capacity / data_size; i++) {
      auto load_success = cache.TryLoad("url" + std::to_string(i), read_buf);
      CHECK(load_success);
    }
    CHECK(read_buf.size() == ((capacity / data_size) * data_size));

    // now is 3 * 6 = 18 bytes, next insert should evict the first guy
    bool cache_success = cache.TryInsert("url4", data);
    CHECK(cache_success);

    // url1 should be evicted
    bool load_success = cache.TryLoad("url1", read_buf);
    CHECK(!load_success);
  }
}

TEST_CASE("[cache_file_test]"){
  const int capacity = 512;
  Cache cache(capacity);
  int total_file_num = 16;

  uint64_t begin = NowSinceEpoch2();
  for (int i = 0; i < total_file_num; i++) {
    std::string file_path = "testfile" + std::to_string(i);
    CHECK(IsFileExists(file_path));
    std::vector<unsigned char> buf;
    LoadFile(file_path, buf);
    bool cache_success = cache.TryInsert(file_path, buf);
    //std::cout << "file content: " << buf.data() << std::endl;
    CHECK(cache_success);
  }
  uint64_t end = NowSinceEpoch2();
  //std::cout << "begin: "<< begin << " end: " << end << std::endl;
  uint64_t spendtime1 = end - begin;
  std::cout << "get data from file, read " << total_file_num << " files," << " spend "<< spendtime1 << " ns." << std::endl;

  int evict_num = 15;
  for (int i = 0; i < evict_num; i++) {
    std::string file_path = "testfile" + std::to_string(i);
    cache.EvictOneByUrl(file_path);
  }

  begin = NowSinceEpoch2();
  int cnt1 = 0, cnt2 = 0;
  for (int i = 0; i < total_file_num; i++) {
    std::string file_path = "testfile" + std::to_string(i);
    std::vector<unsigned char> read_buf;
    bool load_success = cache.TryLoad(file_path, read_buf);
    if(!load_success) {
      cnt2++;
      std::vector<unsigned char> buf;
      LoadFile(file_path, buf);
    }else {
      cnt1++;
    }
    //std::cout << "get file content from memory: " << read_buf.data() << std::endl;
  }
  end = NowSinceEpoch2();
  uint64_t spendtime2 = end - begin;
  //std::cout << "begin: "<< begin << " end: " << end << std::endl;
  std::cout << "get data from memory, read buf " << cnt1 << ", read file " << cnt2  << ", spend "<< spendtime2 << " ns." << std::endl;
  std::cout << "get data from memory, faster than get from file " << (double)spendtime1 / (double)spendtime2 << " times." << std::endl;
}
