#include <iostream>
#include <string>

#include "parser.h"

namespace HFTSystem {

Parser::Parser() {}
Parser::~Parser() {}

void Parser::parseMarketDataMessage(const char *payload) {
  const char msgType = payload[0];

  //   std::cout << "Message "
  //             << " type=" << msgType << " length=" << msgLen << "\n";

  if (msgType == 'A') {
    parseAddOrder(payload);
  }

  // TODO - modify orders , cancel orders, trade events etc
}

void Parser::parseAddOrder(const char *payload) {
  // TODO
  const AddOrder *order = reinterpret_cast<const AddOrder *>(payload + 1);
  (void)order;

  // Order* orderPtr = d_orderMemPool.allocate(Order(.....))
  // d_orderbook.addOrder(orderPtr)

  std::cout << " Parsed Add Order "
            << " Locate code " << ntohs(order->locateCode)
            << " tracking number " << ntohs(order->trackingNumber)
            << " order reference number " << order->orderReferenceNumber
            << " buy sell ?? " << order->buySellIndicator << " num shares "
            << ntohl(order->numShares) << " stock " << std::string(order->stock)
            << " price " << static_cast<double>(ntohl(order->price)) / 10000.0
            << std::endl;

  // TODO - add orderbook updates

  // d_orderbook.addOrder(); // TODO
}

} // namespace HFTSystem