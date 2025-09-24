#ifndef ITCH_TOTALVIEW_5_PARSER_H
#define ITCH_TOTALVIEW_5_PARSER_H

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

  // std::cout << " Parsed Add Order "
  //           << " Locate code " << order->locateCode
  //           << " tracking number " << order->trackingNumber
  //           << " order reference number " << order->orderReferenceNumber
  //           << " buy sell ?? " << order->buySellIndicator
  //           << " num shares " << order->numShares
  //           << " stock " << std::string(order->stock)
  //           << " price " << order->price
  //           << std::endl;

  // TODO - add orderbook updates

  // d_orderbook.addOrder(); // TODO
}

} // namespace HFTSystem

#endif