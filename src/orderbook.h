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
#include "strategy_engine.h"

namespace HFTSystem {

constexpr int NEGATIVE_SIGN = -1;
constexpr int POSITIVE_SIGN = 1;

class Orderbook {
public:
  Orderbook(const std::string &ticker, size_t capacity = 1024 * 64);
  ~Orderbook() {}

  // upserts
  void applyOrderEvent(const AddOrder *addOrder);
  void applyOrderEvent(const CancelOrder *cancelOrder);
  void applyOrderEvent(const PartialCancelOrder *PartialCancelOrder);
  void applyOrderEvent(const FilledOrder *filledOrder);

  // computations for strategies
  double computeImbalance() const;

private:
  void removeOrder(OrderPointer order);

  // computations for strategies
  double topBidVolume() const;
  double topAskVolume() const;
  double topBidPrice() const;
  double topAskPrice() const;

  const std::string d_ticker;

  // memory pool for orders, to reduce heap allocations
  MemPool<Order> d_orderPool;

  // aggregate info about each level / order information
  std::map<Price, LevelData, std::greater<Price>> d_bidLevelData;
  std::map<Price, LevelData, std::less<Price>> d_askLevelData;

  // order tracker
  std::unordered_map<OrderReferenceNumber, OrderPointer>
      d_orders; // makes it more efficient to cancel orders

  // Strategy engine - to pass order imbalance updates and generate OUTCH orders
  std::unique_ptr<StrategyEngine> d_strategyEngine;
};

} // namespace HFTSystem

#endif