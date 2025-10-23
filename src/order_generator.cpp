#include "order_generator.h"

#include <iostream>

namespace HFTSystem {

OrderGenerator::OrderGenerator() {}

void OrderGenerator::submitOrder(Side side, double price,
                                 const std::string &ticker, double quantity) {
  std::cout << "Submitted order " << (side == Side::BUY ? "Buy" : "SELL")
            << ticker << " " << quantity << " " << price << std::endl;

  // increment this per order submitted
  refNumber++;
}

} // namespace HFTSystem
