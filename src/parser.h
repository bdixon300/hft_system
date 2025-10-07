#ifndef PARSER_H
#define PARSER_H

#include "l3_orderbook.h"

namespace HFTSystem {

/**
 * This class will be responsible for parsing ITCH Total 5 view packets
 * into order types for processing by L3 orderbook
 *
 */
class Parser {
public:
  Parser(const std::unordered_map<LocateCode, Ticker> &instrumentMapping);
  ~Parser();

  /** Helper to parse the raw market data payload */
  void parseMarketDataMessage(const char *payload);

private:
  void parseAddOrder(const char *payload);
  void parseCancelOrder(const char *payload);
  void parsePartialCancelOrder(const char *payload);

  // orderbooks
  std::unordered_map<LocateCode, std::unique_ptr<Orderbook>> d_orderbooks;
};

} // namespace HFTSystem

#endif