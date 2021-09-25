//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  std::lock_guard<std::mutex> lk{mtx};
  if (frames.empty()) {
    return false;
  }
  *frame_id = frames.front();
  frames.pop_front();
  map_frame_to_it.erase(*frame_id);
  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  std::lock_guard<std::mutex> lk{mtx};
  auto it = map_frame_to_it.find(frame_id);
  if (it != map_frame_to_it.end()) {
    frames.erase(it->second);
    map_frame_to_it.erase(it);
  }
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  std::lock_guard<std::mutex> lk{mtx};
  auto it = map_frame_to_it.find(frame_id);
  if (it == map_frame_to_it.end()) {
    frames.push_back(frame_id);
    map_frame_to_it[frame_id] = frames.rbegin().base();
  }
  else if (it->second != frames.begin()) {
    /// move frame_id to front of the list
    frames.splice(frames.begin(), frames, it->second);
  }
}

size_t LRUReplacer::Size() {
  std::lock_guard<std::mutex> lk{mtx};
  return frames.size();
}

}  // namespace bustub
