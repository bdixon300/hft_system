#include "strategy_engine.h"

#include <iostream>

namespace HFTSystem {

  StrategyEngine::StrategyEngine() {}

  void StrategyEngine::applyOrderEvent(double orderbookImbalance)
  {
    d_orderbookImbalance = orderbookImbalance;
    std::cout << d_orderbookImbalance << std::endl;

    if (d_orderbookImbalance < d_imbalanceThreshold)
    {
        // SELL order
    }
    else if (d_orderbookImbalance > d_imbalanceThreshold)
    {
        // BUY order
    }
    // if imbalance is 0 dont place orders
  }
}