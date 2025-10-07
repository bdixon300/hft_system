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
  const char msgType = payload[0];

  if (ADD_ORDER_TYPE == msgType) {
    parseOrder<AddOrder>(payload);
  } else if (CANCEL_ORDER_TYPE == msgType) {
    parseOrder<CancelOrder>(payload);
  } else if (PARTIAL_CANCEL_ORDER_TYPE == msgType) {
    parseOrder<PartialCancelOrder>(payload);
  }
  // TODO - implement execute order message
  else {
    // Unsupported order type for parser
  }

  // TODO - modify orders , cancel orders, trade events etc
}

template <typename OrderType>
void Parser::parseOrder(const char* payload)
{
    const OrderType *order = reinterpret_cast<const OrderType *>(payload + 1); 
    const auto &iter = d_orderbooks.find(ntohs(order->locateCode));

    if (iter != d_orderbooks.end()) {
        iter->second->orderUpsert(order);
        //std::cout << " Parsed Order " << " Locate code " << ntohs(order->locateCode);
    }
}

} // namespace HFTSystem