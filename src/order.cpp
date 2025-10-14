#include "order.h"

namespace HFTSystem {

Order::Order(OrderReferenceNumber order, Price price, Side side,
             Quantity quantity)
    : d_orderReferenceNumber(order), d_price(price), d_side(side),
      d_initialQuantity(quantity), d_remainingQuantity(quantity),
      d_orderType(OrderType::LIMIT) {}

Order::Order(const AddOrder *addOrder)
    : d_orderReferenceNumber(addOrder->orderReferenceNumber),
      d_price(static_cast<double>(ntohl(addOrder->price)) / 10000.0),
      d_side(addOrder->buySellIndicator == 'B' ? Side::BUY : Side::SELL),
      d_initialQuantity(ntohl(addOrder->numShares)),
      d_remainingQuantity(ntohl(addOrder->numShares)),
      d_orderType(OrderType::LIMIT) {}

bool Order::filledOrCancelled() { return d_remainingQuantity <= 0; }

void Order::partialCancel(Quantity quantity) {
  d_remainingQuantity -= quantity;
}

Price Order::getPrice() const { return d_price; }

OrderReferenceNumber Order::getOrderReferenceNumber() const {
  return d_orderReferenceNumber;
}

Quantity Order::getQuantity() const { return d_initialQuantity; }

Quantity Order::getRemainingQuantity() const { return d_remainingQuantity; }

Side Order::getSide() const { return d_side; }

OrderType Order::getOrderType() const { return d_orderType; }

void Order::setPrice(Price price) { d_price = price; }

} // namespace HFTSystem