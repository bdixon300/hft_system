#include "order_generator.h"

#include <iostream>

namespace HFTSystem {

OrderGenerator::OrderGenerator() {}

void OrderGenerator::submitOrder(Side side, double price,
                                 const std::string &ticker, double quantity) {

  OutboundOrder order{};
  order.type = 'O';
  order.userReferenceNumber = refNumber;
  order.side = Side::BUY == side ? 'B' : 'S';
  order.executedShares = static_cast<uint32_t>(quantity);
  order.price = htonl(static_cast<uint32_t>(price * 10000));
  std::strncpy(order.symbol, ticker.c_str(), sizeof(order.symbol));

  // Future deliverable - establish a TCP connection to a simulated exchange connection to submit
  // Outch payload
  std::cout << "Submitted order " << order.userReferenceNumber << std::endl;

  // increment this per order submitted
  refNumber++;
}

} // namespace HFTSystem
