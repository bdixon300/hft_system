#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>

#include "mempool.h"
#include "order.h"

namespace HFTSystem {

class Orderbook {
public:
  Orderbook(size_t capacity = 1024);
  ~Orderbook() {}

  bool isOrderFullyFillable(const OrderPointer &order);
  void addOrder(const OrderPointer &order);
  void cancelOrder(const OrderPointer &order);

private:
  // memory pool for orders, to reduce heap allocations
  MemPool<Order> d_orderPool;

  // actual price time priority order maps
  std::map<Price, OrderPointers, std::greater<Price>> m_bids;
  std::map<Price, OrderPointers, std::less<Price>> m_asks;

  // aggregate info about each level / order information
  std::unordered_map<Price, LevelData> m_bidLevelData;
  std::unordered_map<Price, LevelData> m_askLevelData;

  // order tracker
  std::unordered_map<OrderId, OrderEntry>
      m_orders; // makes it more efficient to cancel orders

  // thread safety
  std::mutex m_orderMutex;
};

} // namespace HFTSystem

#endif