#ifndef ORDER_GENERATOR_H
#define ORDER_GENERATOR_H

#include <cstdint>
#include <string>

#include "order.h"

namespace HFTSystem {

/**
 * Implementation of a basic OUTCH based order generator.
 * Generates OUTCH messages to the exchange for processing
 */
class OrderGenerator {
public:
  OrderGenerator();
  ~OrderGenerator() {}

  void submitOrder(Side side, const std::string &ticker, double quantity);

private:
};

} // namespace HFTSystem

#endif