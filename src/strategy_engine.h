#ifndef STRATEGY_ENGINE_H
#define STRATEGY_ENGINE_H

#include <cstdint>
#include <optional>
#include <string>

#include "order_generator.h"

namespace HFTSystem {

/**
 * Implementation of a basic market maker/
 * Orderbook imbalance strategy Engine
 */
class StrategyEngine {
public:
  StrategyEngine(const std::string &ticker,
                 double orderImbalanceThreshold = 0.2);

  ~StrategyEngine() {}

  void applyOrderEvent(double orderbookImbalance, double bestBidPrice,
                       double bestAskPrice);

private:
  int calculateOrderQuantity(double imbalance, int minQuantity = 1,
                             int maxQuantity = 1000);

  const std::string d_ticker;
  double d_imbalanceThreshold;
  OrderGenerator d_orderGenerator;
};

} // namespace HFTSystem

#endif