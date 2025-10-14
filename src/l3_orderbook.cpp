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

  std::cout << " Cancelling Order: " << d_ticker << "  "
            << orderEntry.order->getOrderReferenceNumber() << " "
            << orderEntry.order->getPrice() << " "
            << orderEntry.order->getQuantity() << std::endl;

  removeOrder(orderEntry);
}

void Orderbook::applyOrderEvent(const PartialCancelOrder *partialCancelOrder) {
  // if we dont have an order in book matching we have to ignore
  if (!d_orders.count(partialCancelOrder->orderReferenceNumber)) {
    std::cout << "No corresponding order, here for ref number: "
              << partialCancelOrder->orderReferenceNumber << ", skipping trade"
              << std::endl;
    return;
  }

  OrderEntry &orderEntry = d_orders[partialCancelOrder->orderReferenceNumber];

  std::cout << " Partially Cancelling Order: " << d_ticker << "  "
            << orderEntry.order->getOrderReferenceNumber() << " "
            << orderEntry.order->getPrice()
            << " Outstanding quantity: " << orderEntry.order->getQuantity()
            << ", Quantity to cancel: " << partialCancelOrder->numShares
            << std::endl;

  orderEntry.order->partialCancel(partialCancelOrder->numShares);

  // If order is fully cancelled remove from orderbook
  if (orderEntry.order->filledOrCancelled()) {
    removeOrder(orderEntry);
  }
}

void Orderbook::removeOrder(OrderEntry &orderEntry) {
  OrderPointer &order = orderEntry.order;

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
  // Remove order from memory pool
  d_orderPool.deallocate(order);
}

} // namespace HFTSystem