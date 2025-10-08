#include "l3_orderbook.h"

namespace HFTSystem {

// TODO

Orderbook::Orderbook(const std::string &ticker, size_t capacity)
    : d_ticker(ticker), d_orderPool(capacity) {}

void Orderbook::applyOrderEvent(const AddOrder *addOrder) {
  //addOrder(addOrder);
    (void)addOrder;
}

void Orderbook::applyOrderEvent(const CancelOrder *cancelOrder) {
  // TODO
  //cancelOrder(cancelOrder);
    (void)cancelOrder;
}

void Orderbook::applyOrderEvent(
    const PartialCancelOrder *partialCancelOrder) {
  // TODO
  //partialCancelOrder(partialCancelOrder);
    (void)partialCancelOrder;
}

} // namespace HFTSystem