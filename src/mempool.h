#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "order.h"

namespace HFTSystem {

// Memory pool for Order objects
template <typename T> class MemPool {
public:
  MemPool(size_t capacity)
      : d_capacity(capacity), d_freeIndices(capacity), d_nextFree(0) {
    // Allocate raw memory for orders
    d_pool = operator new[](sizeof(T) * capacity);

    // Initialize free list
    for (size_t i = 0; i < capacity; ++i) {
      d_freeIndices[i] = i;
    }
  }

  ~MemPool() {
    size_t liveCount = d_nextFree;
    for (size_t i = 0; i < liveCount; ++i) {
      size_t index = d_freeIndices[i];
      T *obj = getPointer(index);
      obj->~T();
    }
    operator delete[](d_pool);
  }

  // Allocate a new Order from the pool
  template <typename... Args> T *allocate(Args &&...args) {
    if (d_nextFree == d_capacity) {
      throw std::runtime_error("Memory Pool exhausted");
    }
    size_t index = d_freeIndices[d_nextFree++];
    T *t = new (getPointer(index)) T(std::forward<Args>(args)...);
    return t;
  }

  // Deallocate an order from the pool
  void deallocate(T *t) {
    if (!t)
      return;

    // Call destructor explicitly
    t->~T();

    ptrdiff_t index =
        reinterpret_cast<char *>(t) - reinterpret_cast<char *>(d_pool);
    index /= sizeof(T);
    if (index < 0 || static_cast<size_t>(index) >= d_capacity)
      throw std::runtime_error("Invalid pointer deallocation");

    d_freeIndices[--d_nextFree] = static_cast<size_t>(index);
  }
  // Reset pool (optional, for testing / clearing all)
  void reset() { d_nextFree = 0; }

private:
  // helper to get address for current index
  T *getPointer(size_t index) {
    return reinterpret_cast<T *>(reinterpret_cast<char *>(d_pool) +
                                 index * sizeof(T));
  }

  size_t d_capacity;
  void *d_pool;
  std::vector<size_t> d_freeIndices;
  size_t d_nextFree;
};

} // namespace HFTSystem

#endif