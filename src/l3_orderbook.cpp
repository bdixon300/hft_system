#include "l3_orderbook.h"

namespace HFTSystem {

// TODO

Orderbook::Orderbook(const std::string &ticker, size_t capacity)
    : d_ticker(ticker), d_orderPool(capacity) {}

void Orderbook::orderUpsert(const AddOrder *addOrder) {
  //addOrder(addOrder);
    (void)addOrder;
}

void Orderbook::orderUpsert(const CancelOrder *cancelOrder) {
  // TODO
  //cancelOrder(cancelOrder);
    (void)cancelOrder;
}

void Orderbook::orderUpsert(
    const PartialCancelOrder *partialCancelOrder) {
  // TODO
  //partialCancelOrder(partialCancelOrder);
    (void)partialCancelOrder;
}

void Orderbook::addOrder(const AddOrder *addOrder) {
  // TODO
  (void)addOrder;
  // d_orderPool.allocate();
}

void Orderbook::cancelOrder(const CancelOrder *cancelOrder) {
  // TODO
  (void)cancelOrder;
}

void Orderbook::partialCancelOrder(
    const PartialCancelOrder *partialCancelOrder) {
  // TODO
  (void)partialCancelOrder;
}

} // namespace HFTSystem