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

  if (msgType == 'A') {
    parseAddOrder(payload);
  } else if (msgType == 'D') {
    parseCancelOrder(payload);
  } else if (msgType == 'X') {
    parsePartialCancelOrder(payload);
  }
  // TODO - implement execute order message
  else {
    // Unsupported order type for parser
  }

  // TODO - modify orders , cancel orders, trade events etc
}

void Parser::parseAddOrder(const char *payload) {
  const AddOrder *order = reinterpret_cast<const AddOrder *>(payload + 1);

  const auto &iter = d_orderbooks.find(ntohs(order->locateCode));
  if (iter != d_orderbooks.end()) {
    std::cout << " Parsed Add Order "
              << " Locate code " << ntohs(order->locateCode)
              << " tracking number " << ntohs(order->trackingNumber)
              << " order reference number " << order->orderReferenceNumber
              << " buy sell ?? " << order->buySellIndicator << " num shares "
              << ntohl(order->numShares) << " stock "
              << std::string(order->stock) << " price "
              << static_cast<double>(ntohl(order->price)) / 10000.0
              << std::endl;

    iter->second->addOrder(order);
  }
}

void Parser::parseCancelOrder(const char *payload) {
  // TODO
  const CancelOrder *order = reinterpret_cast<const CancelOrder *>(payload + 1);

  const auto &iter = d_orderbooks.find(ntohs(order->locateCode));
  if (iter != d_orderbooks.end()) {
    std::cout << " Parsed Cancel Order "
              << " Locate code " << ntohs(order->locateCode)
              << " tracking number " << ntohs(order->trackingNumber)
              << " order reference number " << order->orderReferenceNumber
              << std::endl;
    iter->second->cancelOrder(order);
  }
}

void Parser::parsePartialCancelOrder(const char *payload) {
  // TODO
  const PartialCancelOrder *order =
      reinterpret_cast<const PartialCancelOrder *>(payload + 1);

  const auto &iter = d_orderbooks.find(ntohs(order->locateCode));
  if (iter != d_orderbooks.end()) {
    std::cout << " Parsed Partial Cancel Order "
              << " Locate code " << ntohs(order->locateCode)
              << " tracking number " << ntohs(order->trackingNumber)
              << " order reference number " << order->orderReferenceNumber
              << " num shares " << ntohl(order->numShares) << std::endl;
    iter->second->partialCancelOrder(order);
  }
}

} // namespace HFTSystem