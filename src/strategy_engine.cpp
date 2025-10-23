#include "strategy_engine.h"
#include "order.h"

#include <iostream>

namespace HFTSystem {

StrategyEngine::StrategyEngine(const std::string &ticker,
                               double imbalanceThreshold)
    : d_ticker(ticker), d_imbalanceThreshold(imbalanceThreshold) {}

void StrategyEngine::applyOrderEvent(double orderbookImbalance) {
  if (std::abs(orderbookImbalance) < d_imbalanceThreshold) {
    // too small an imbalance, don't trade
    return;
  }

  double quantity = calculateOrderQuantity(orderbookImbalance);

  if (orderbookImbalance < d_imbalanceThreshold) {
    // SELL order
    d_orderGenerator.submitOrder(Side::BUY, d_ticker, quantity);
  } else if (orderbookImbalance > d_imbalanceThreshold) {
    // BUY order
    d_orderGenerator.submitOrder(Side::SELL, d_ticker, quantity);
  }
  // if imbalance is 0 dont place orders
}

int StrategyEngine::calculateOrderQuantity(double imbalance, int minQuantity,
                                           int maxQuantity) {
  double scaling_factor = (std::abs(imbalance) - d_imbalanceThreshold) /
                          (1.0 - d_imbalanceThreshold);
  if (scaling_factor < 0.0)
    scaling_factor = 0.0;
  if (scaling_factor > 1.0)
    scaling_factor = 1.0;

  int qty = static_cast<int>(
      std::round(minQuantity + scaling_factor * (maxQuantity - minQuantity)));
  qty = std::clamp(qty, minQuantity, maxQuantity);
  return qty;
}
} // namespace HFTSystem