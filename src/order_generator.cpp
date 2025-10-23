#include "order_generator.h"

#include <iostream>

namespace HFTSystem {

OrderGenerator::OrderGenerator() {}

void OrderGenerator::submitOrder(Side side, const std::string &ticker,
                                 double quantity) {
  // TODO
  (void)side;
  (void)ticker;
  (void)quantity;

  // std::cout << "Submitted order " << (side == Side::BUY ? "Buy" : "SELL") <<
  // ticker << " " << quantity << std::endl;
}

} // namespace HFTSystem
