#include <iostream>
#include <string>

#include "parser.h"

namespace HFTSystem {

Parser::Parser(
    const std::unordered_map<LocateCode, Ticker> &instrumentMapping) {
  // Prefill Tickers/instruments we want to track, added 3 for experimental
  // purposes
  for (const auto &[locateCode, ticker] : instrumentMapping) {
    d_orderbooks.insert({locateCode, std::make_unique<Orderbook>(ticker)});
  }
}

Parser::~Parser() {}

void Parser::parseMarketDataMessage(const char *payload) {
  const char& msgType = payload[0];

  // Order messages mostly cancels and skewness encourages branch prediction
  // For < 10 msg types if else faster than switch and much fast than
  // a map to functions to call
  switch (msgType) {
  case CANCEL_ORDER_TYPE:
    [[likely]] parseOrder<CancelOrder>(payload);
    break;
  case ADD_ORDER_TYPE:
    parseOrder<AddOrder>(payload);
    break;
  case PARTIAL_CANCEL_ORDER_TYPE:
    parseOrder<PartialCancelOrder>(payload);
    break;
  case FILLED_ORDER_TYPE:
    parseOrder<FilledOrder>(payload);
    break;
  default:
    // Unknown order cannot be parsed
    break;
  }
}

template <typename OrderType> void Parser::parseOrder(const char *payload) {
  const OrderType *order = reinterpret_cast<const OrderType *>(payload + 1);
  const auto &iter = d_orderbooks.find(ntohs(order->locateCode));

  if (iter != d_orderbooks.end()) {
    iter->second->applyOrderEvent(order);
    // std::cout << " Parsed Order " << " Locate code " <<
    // ntohs(order->locateCode);
  }
}

} // namespace HFTSystem