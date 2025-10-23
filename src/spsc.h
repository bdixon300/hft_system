#ifndef MPSC_H
#define MPSC_H

#include <atomic>
#include <optional>
#include <vector>

namespace HFTSystem {

// Lock free thread safe single producer single consumer queue
// for low latency data transfer between threads
// I implement the SPSC as a circular ring buffer
// I think some of the memory ordering operations are overkill
// here but need to research this topic futher
template <typename T> class SPSC {
public:
  explicit SPSC(size_t capacity)
      : d_capacity(capacity), d_data(capacity), d_head(0), d_tail(0) {}
  ~SPSC() = default;

  bool try_push(const T &data) {
    const std::size_t tail = d_tail.fetch_add(1, std::memory_order_acq_rel);
    DataBlock &dataBlock = d_data[tail % d_capacity];

    // if full we cannot push. Client should retry (allows for busy waiting)
    if (dataBlock.ready.load(std::memory_order_acquire)) {
      return false;
    }

    dataBlock.data = data;
    dataBlock.ready.store(true, std::memory_order_release);
    return true;
  }

  std::optional<T> try_pop() {
    const std::size_t head = d_head.load(std::memory_order_relaxed);
    DataBlock &dataBlock = d_data[head % d_capacity];

    if (!dataBlock.ready.load(std::memory_order_acquire)) {
      return std::nullopt; // queue empty
    }

    T value = std::move(dataBlock.data);
    dataBlock.ready.store(false, std::memory_order_release);
    d_head.store((head + 1) % d_capacity, std::memory_order_relaxed);
    return value;
  }

private:
  struct DataBlock {
    T data;
    std::atomic<bool> ready{false};
  };

  size_t d_capacity;
  std::vector<DataBlock> d_data;
  std::atomic<size_t> d_head;
  std::atomic<size_t> d_tail;
};

} // namespace HFTSystem

#endif