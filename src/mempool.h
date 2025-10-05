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
      : m_capacity(capacity), m_freeIndices(capacity), m_nextFree(0) {
    // Allocate raw memory for orders
    m_pool = operator new[](sizeof(T) * capacity);

    // Initialize free list
    for (size_t i = 0; i < capacity; ++i) {
      m_freeIndices[i] = i;
    }
  }

  // Allocate a new Order from the pool
  template <typename... Args> T *allocate(Args &&...args) {
    if (m_nextFree == m_capacity) {
      throw std::runtime_error("Memory Pool exhausted");
    }
    size_t index = m_freeIndices[m_nextFree++];
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
        reinterpret_cast<char *>(t) - reinterpret_cast<char *>(m_pool);
    index /= sizeof(t);
    if (index < 0 || static_cast<size_t>(index) >= m_capacity)
      throw std::runtime_error("Invalid pointer deallocation");

    m_freeIndices[--m_nextFree] = static_cast<size_t>(index);
  }

  // Reset pool (optional, for testing / clearing all)
  void reset() { m_nextFree = 0; }

private:
  // helper to get address for current index
  T *getPointer(size_t index) {
    return reinterpret_cast<T *>(reinterpret_cast<char *>(m_pool) +
                                 index * sizeof(T));
  }

  size_t m_capacity;
  void *m_pool;
  std::vector<size_t> m_freeIndices;
  size_t m_nextFree;
};

} // namespace HFTSystem

#endif