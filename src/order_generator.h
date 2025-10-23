#ifndef ORDER_GENERATOR_H
#define ORDER_GENERATOR_H

#include <cstdint>
#include <string>

#include "order.h"

namespace HFTSystem {

/**
 * OUTCH BASED structs
 *
 */
#pragma pack(push, 1)
struct OutboundOrder {
  char type;
  u_int32_t userReferenceNumber;
  char side;
  uint32_t executedShares;
  char symbol[8];
  uint64_t price;
  // There are other fields but for hobby project i will omit these
};
#pragma pack(pop)

/**
 * Implementation of a basic OUTCH based order generator.
 * Generates OUTCH messages to the exchange for processing.
 * In the next phase of the project, I will extend this to
 * make a TCP connection to an exchange simulator, but for now
 * it will just log the order to stdout
 * output file.
 */
class OrderGenerator {
public:
  OrderGenerator();
  ~OrderGenerator() {}

  void submitOrder(Side side, double price, const std::string &ticker,
                   double quantity);

private:
  uint32_t refNumber{0};
};

} // namespace HFTSystem

#endif