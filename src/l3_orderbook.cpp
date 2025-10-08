#include "l3_orderbook.h"
#include <iostream>

namespace HFTSystem {

// TODO

Orderbook::Orderbook(const std::string &ticker, size_t capacity)
    : d_ticker(ticker), d_orderPool(capacity) {}

void Orderbook::applyOrderEvent(const AddOrder *addOrder) {
  OrderPointer order = d_orderPool.allocate(addOrder);

  std::cout << " Added Order: " << d_ticker << "  " << order->getOrderReferenceNumber() << " "
            << order->getPrice() << " " << order->getQuantity() << std::endl;

  // add order object to map etc
}

void Orderbook::applyOrderEvent(const CancelOrder *cancelOrder) {
  // TODO
  // cancelOrder(cancelOrder);
  (void)cancelOrder;
}

void Orderbook::applyOrderEvent(const PartialCancelOrder *partialCancelOrder) {
  // TODO
  // partialCancelOrder(partialCancelOrder);
  (void)partialCancelOrder;
}

} // namespace HFTSystem