#include "order.h"

namespace HFTSystem {
// TODO

bool Order::filled() {
  // TODO
  return false;
}

void Order::fill(Quantity quantity) {
  (void)quantity;
  // TODO
}

Price Order::getPrice() const { return m_price; }

OrderId Order::getOrderId() const { return m_orderId; }

Quantity Order::getQuantity() const { return m_initialQuantity; }

Quantity Order::getRemainingQuantity() const { return m_remainingQuantity; }

Side Order::getSide() const { return m_side; }

OrderType Order::getOrderType() const { return m_orderType; }

void Order::setPrice(Price price) { m_price = price; }

} // namespace HFTSystem