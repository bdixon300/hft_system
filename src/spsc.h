#ifndef SPSC_H
#define SPSC_H

namespace HFTSystem {

// Lock free thread safe single producer single consumer queue
// for low latency data transfer between threads
// I implement the SPSC as a circular ring buffer
template <typename T> class SPSC {
public:
  explicit SPSC(size_t capacity) : d_capacity(capacity), {}
  ~SPSC() {}

  void push_back(T &data) {
    // TODO
  }

  void push_front(T &data) {
    // TODO
  }

  T pop_front() {
    // TODO
  }

  T pop_back() {
    // TODO
  }

private:
  size_t d_capacity;
  std::vector<T> d_data;
  std::atomic<size_t> d_head;
  std::atomic<size_t> d_tail;
};

} // namespace HFTSystem

#endif