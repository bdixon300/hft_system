#include "l3_orderbook.h"
#include <iostream>

namespace HFTSystem {

Orderbook::Orderbook(const std::string &ticker, size_t capacity)
    : d_ticker(ticker), d_orderPool(capacity) {}

void Orderbook::applyOrderEvent(const AddOrder *addOrder) {
  OrderPointer order = d_orderPool.allocate(addOrder);

  std::cout << " Added Order: " << d_ticker << "  "
            << order->getOrderReferenceNumber() << " " << order->getPrice()
            << " " << order->getQuantity() << std::endl;

  if (order->getSide() == Side::BUY) {
    auto &bids = d_bids[order->getPrice()];
    bids.emplace_back(order);
    d_orders.insert({order->getOrderReferenceNumber(),
                     OrderEntry{order, std::prev(bids.end())}});
    d_bidLevelData[order->getPrice()].totalQuantity += order->getQuantity();
  } else {
    auto &asks = d_asks[order->getPrice()];
    asks.emplace_back(order);
    d_orders.insert({order->getOrderReferenceNumber(),
                     OrderEntry{order, std::prev(asks.end())}});
    d_askLevelData[order->getPrice()].totalQuantity += order->getQuantity();
  }
}

void Orderbook::applyOrderEvent(const CancelOrder *cancelOrder) {

  // if we dont have an order in book matching we have to ignore
  if (!d_orders.count(cancelOrder->orderReferenceNumber)) {
    std::cout << "No corresponding order, here for ref number: "
              << cancelOrder->orderReferenceNumber << ", skipping trade"
              << std::endl;
    return;
  }

  OrderEntry &orderEntry = d_orders[cancelOrder->orderReferenceNumber];
  OrderPointer &order = orderEntry.order;

  std::cout << " Cancelling Order: " << d_ticker << "  "
            << order->getOrderReferenceNumber() << " " << order->getPrice()
            << " " << order->getQuantity() << std::endl;

  if (order->getSide() == Side::BUY) {
    // level data update
    d_bidLevelData[order->getPrice()].totalQuantity -=
        order->getRemainingQuantity();

    d_bids[order->getPrice()].erase(orderEntry.location_);
    if (d_bids[order->getPrice()].empty())
      d_bids.erase(order->getPrice());
  } else {
    // level data update
    d_askLevelData[order->getPrice()].totalQuantity -=
        order->getRemainingQuantity();

    d_asks[order->getPrice()].erase(orderEntry.location_);
    if (d_asks[order->getPrice()].empty())
      d_asks.erase(order->getPrice());
  }

  d_orders.erase(order->getOrderReferenceNumber());
  d_orderPool.deallocate(order);
  (void)cancelOrder;
}

void Orderbook::applyOrderEvent(const PartialCancelOrder *partialCancelOrder) {
  // TODO
  (void)partialCancelOrder;
}

} // namespace HFTSystem