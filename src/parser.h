#ifndef PARSER_H
#define PARSER_H

#include "l3_orderbook.h"

namespace HFTSystem {

/*
    Order types currently supported (not exhaustive)
*/
#pragma pack(push, 1)
struct AddOrder {
  uint16_t locateCode;
  uint16_t trackingNumber;
  char timestamp[6];
  uint64_t orderReferenceNumber;
  uint8_t buySellIndicator;
  uint32_t numShares;
  char stock[8];
  uint32_t price;
};
#pragma pack(pop)

struct AddOrderMpid {
  // TODO
};

struct ModifyOrder {
  // TODO
};

struct CancelOrder {
  // TODO
};

/**
 * This class will be responsible for parsing ITCH Total 5 view packets
 * into order types for processing by L3 orderbook
 *
 */
class Parser {
public:
  Parser();
  ~Parser();

  /** Helper to parse the raw market data payload */
  void parseMarketDataMessage(const char *payload);

private:
  void parseAddOrder(const char *payload);

  /** TODO - add Helpers by ordertype */

  Orderbook d_orderbook;
};

} // namespace HFTSystem

#endif