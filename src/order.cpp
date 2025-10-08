#include "order.h"

namespace HFTSystem {

Order::Order(OrderReferenceNumber order, Price price, Side side,
             Quantity quantity)
    : m_orderReferenceNumber(order), m_price(price), m_side(side),
      m_initialQuantity(quantity), m_remainingQuantity(quantity),
      m_orderType(OrderType::LIMIT) {}

Order::Order(const AddOrder *addOrder)
    : m_orderReferenceNumber(addOrder->orderReferenceNumber),
      m_price(static_cast<double>(ntohl(addOrder->price)) / 10000.0),
      m_side(addOrder->buySellIndicator == 'B' ? Side::BUY : Side::SELL),
      m_initialQuantity(ntohl(addOrder->numShares)),
      m_remainingQuantity(ntohl(addOrder->numShares)),
      m_orderType(OrderType::LIMIT) {}

bool Order::filled() {
  // TODO
  return false;
}

void Order::fill(Quantity quantity) {
  (void)quantity;
  // TODO
}

Price Order::getPrice() const { return m_price; }

OrderReferenceNumber Order::getOrderReferenceNumber() const {
  return m_orderReferenceNumber;
}

Quantity Order::getQuantity() const { return m_initialQuantity; }

Quantity Order::getRemainingQuantity() const { return m_remainingQuantity; }

Side Order::getSide() const { return m_side; }

OrderType Order::getOrderType() const { return m_orderType; }

void Order::setPrice(Price price) { m_price = price; }

} // namespace HFTSystem