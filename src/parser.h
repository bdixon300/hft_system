#ifndef PARSER_H
#define PARSER_H

#include "l3_orderbook.h"

namespace HFTSystem {

constexpr char ADD_ORDER_TYPE = 'A';
constexpr char CANCEL_ORDER_TYPE = 'D';
constexpr char PARTIAL_CANCEL_ORDER_TYPE = 'X';

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

  // Helper to extract binary stream and pass it to appropriate orderbook
  template <typename T>
  void parseOrder(const char *payload);

  // orderbooks
  std::unordered_map<LocateCode, std::unique_ptr<Orderbook>> d_orderbooks;
};

} // namespace HFTSystem

#endif