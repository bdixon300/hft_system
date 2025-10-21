#ifndef STRATEGY_ENGINE_H
#define STRATEGY_ENGINE_H

#include <cstdint>
#include <optional>

namespace HFTSystem {

/**
 * Implementation of a basic market maker/
 * Orderbook imbalance strategy Engine
 */
class StrategyEngine {
public:
  StrategyEngine();
  ~StrategyEngine(){}

  void applyOrderEvent(double orderbookImbalance);

private:
   double d_orderbookImbalance;
   double d_imbalanceThreshold;

  // OrderGenerator d_orderGenerator;
};

} // namespace HFTSystem

#endif