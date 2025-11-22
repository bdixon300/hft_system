#ifndef MPSC_H
#define MPSC_H

#include <atomic>
#include <optional>
#include <vector>

namespace HFTSystem {

// Lock free thread safe single producer single consumer queue
// for low latency data transfer between threads
// I implement the SPSC as a circular ring buffer
static constexpr uint16_t CACHE_LINE = 64;

template <typename T> class SPSCQ {
public:
  SPSCQ(size_t capacity)
      : d_capacity(capacity), d_mask(capacity - 1), d_head(0), d_tail(0),
        d_data(capacity) {
    if (d_capacity <= 2 || (d_capacity & (d_capacity - 1) != 0))
      throw std::runtime_error("Capacity violation");
  }

  template <typename U> bool try_emplace(U &&item) {
    size_t cur_tail = d_tail.load(std::memory_order_relaxed);

    // full check return false
    if (((cur_tail + 1) & d_mask) == d_head.load(std::memory_order_acquire))
      return false;

    // assign element to slot
    d_data[cur_tail] = std::forward<U>(item);

    // update tail position
    d_tail.store((cur_tail + 1) & d_mask, std::memory_order_release);

    return true;
  }

  bool push(const T &item) { return try_emplace(item); }

  bool push(const T &&item) { return try_emplace(std::move(item)); }

  bool pop(T &item) {
    size_t cur_head = d_head.load(std::memory_order_relaxed);

    // empty check
    if (cur_head == d_tail.load(std::memory_order_acquire))
      return false;

    // assign to out param
    item = d_data[cur_head];

    // update head position
    d_head.store((cur_head + 1) & d_mask, std::memory_order_release);

    return true;
  }

  [[nodiscard]] bool full() const {
    size_t cur_head = d_head.load(std::memory_order_acquire);
    size_t cur_tail = d_tail.load(std::memory_order_relaxed);

    return ((cur_tail + 1) & d_mask) == cur_head;
  }

  [[nodiscard]] size_t size() const {
    size_t cur_head = d_head.load(std::memory_order_acquire);
    size_t cur_tail = d_tail.load(std::memory_order_acquire);
    return (cur_tail - cur_head) & d_mask;
  }

  [[nodiscard]] bool empty() const {
    size_t cur_head = d_head.load(std::memory_order_relaxed);
    size_t cur_tail = d_tail.load(std::memory_order_acquire);

    return cur_head == cur_tail;
  }

private:
  alignas(CACHE_LINE) std::atomic<size_t> d_head;
  alignas(CACHE_LINE) std::atomic<size_t> d_tail;

  std::vector<T> d_data;
  const size_t d_capacity;
  const size_t d_mask;
};

} // namespace HFTSystem

#endif