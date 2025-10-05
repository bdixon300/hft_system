#include "l3_orderbook.h"

namespace HFTSystem {

// TODO

Orderbook::Orderbook(size_t capacity) : d_orderPool(capacity) {}

bool Orderbook::isOrderFullyFillable(const OrderPointer &order) {
  // TODO
  (void)order;
  return false;
}

void Orderbook::addOrder(const OrderPointer &order) {
  // TODO
  (void)order;
}

void Orderbook::cancelOrder(const OrderPointer &order) {
  // TODO
  (void)order;
}

} // namespace HFTSystem