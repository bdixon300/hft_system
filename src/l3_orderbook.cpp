#include "l3_orderbook.h"
#include <iostream>

namespace HFTSystem {

Orderbook::Orderbook(const std::string &ticker, size_t capacity)
    : d_ticker(ticker), d_orderPool(capacity),
      d_strategyEngine(std::make_unique<StrategyEngine>(ticker)) {}

double Orderbook::computeImbalance() const {
  double vb = topBidVolume();
  double va = topAskVolume();
  if (vb + va == 0)
    return 0.0;
  return (vb - va) / (vb + va);
}

double Orderbook::topBidVolume() const {
  return d_bidLevelData.empty() ? 0.0
                                : d_bidLevelData.begin()->second.totalQuantity;
}

double Orderbook::topAskVolume() const {
  return d_askLevelData.empty() ? 0.0
                                : d_askLevelData.begin()->second.totalQuantity;
}

double Orderbook::topBidPrice() const {
  return d_bidLevelData.empty() ? 0.0 : d_bidLevelData.begin()->first;
}

double Orderbook::topAskPrice() const {
  return d_askLevelData.empty() ? 0.0 : d_askLevelData.begin()->first;
}

void Orderbook::applyOrderEvent(const AddOrder *addOrder) {
  OrderPointer order = d_orderPool.allocate(addOrder);

  //   std::cout << " Added Order: " << d_ticker << "  "
  //             << order->getOrderReferenceNumber() << " " << order->getPrice()
  //             << " " << order->getQuantity() << std::endl;

  if (order->getSide() == Side::BUY) {
    auto &bids = d_bids[order->getPrice()];
    bids.emplace_back(order);
    d_orders.insert({order->getOrderReferenceNumber(),
                     OrderEntry{order, std::prev(bids.end())}});
  } else {
    auto &asks = d_asks[order->getPrice()];
    asks.emplace_back(order);
    d_orders.insert({order->getOrderReferenceNumber(),
                     OrderEntry{order, std::prev(asks.end())}});
  }

  updateLevelData(order->getSide(), order->getPrice(),
                  order->getRemainingQuantity());

  // Strategy engine
  d_strategyEngine->applyOrderEvent(computeImbalance(), topBidPrice(),
                                    topAskPrice());
}

void Orderbook::applyOrderEvent(const CancelOrder *cancelOrder) {
  // if we dont have an order in book matching we have to ignore
  if (!d_orders.count(cancelOrder->orderReferenceNumber)) {
    // std::cout << "No corresponding order, here for ref number: "
    //           << cancelOrder->orderReferenceNumber << ", skipping trade"
    //           << std::endl;
    return;
  }

  OrderEntry &orderEntry = d_orders[cancelOrder->orderReferenceNumber];

  //   std::cout << " Cancelling Order: " << d_ticker << "  "
  //             << orderEntry.order->getOrderReferenceNumber() << " "
  //             << orderEntry.order->getPrice() << " "
  //             << orderEntry.order->getQuantity() << std::endl;

  updateLevelData(orderEntry.order->getSide(), orderEntry.order->getPrice(),
                  -orderEntry.order->getRemainingQuantity());
  removeOrder(orderEntry);

  // Strategy engine
  d_strategyEngine->applyOrderEvent(computeImbalance(), topBidPrice(),
                                    topAskPrice());
}

void Orderbook::applyOrderEvent(const PartialCancelOrder *partialCancelOrder) {
  // if we dont have an order in book matching we have to ignore
  if (!d_orders.count(partialCancelOrder->orderReferenceNumber)) {
    // std::cout << "No corresponding order, here for ref number: "
    //           << partialCancelOrder->orderReferenceNumber << ", skipping
    //           trade"
    //           << std::endl;
    return;
  }

  OrderEntry &orderEntry = d_orders[partialCancelOrder->orderReferenceNumber];

  //   std::cout << " Partially Cancelling Order: " << d_ticker << "  "
  //             << orderEntry.order->getOrderReferenceNumber() << " "
  //             << orderEntry.order->getPrice()
  //             << " Outstanding quantity: " << orderEntry.order->getQuantity()
  //             << ", Quantity to cancel: " << partialCancelOrder->numShares
  //             << std::endl;

  orderEntry.order->partialCancel(ntohl(partialCancelOrder->numShares));

  updateLevelData(orderEntry.order->getSide(), orderEntry.order->getPrice(),
                  -orderEntry.order->getRemainingQuantity());
  // If order is fully cancelled remove from orderbook
  if (orderEntry.order->filledOrCancelled()) {
    removeOrder(orderEntry);
  }

  // Strategy engine
  d_strategyEngine->applyOrderEvent(computeImbalance(), topBidPrice(),
                                    topAskPrice());
}

void Orderbook::applyOrderEvent(const FilledOrder *filledOrder) {
  // if we dont have an order in book matching we have to ignore
  if (!d_orders.count(filledOrder->orderReferenceNumber)) {
    // std::cout << "No corresponding order, here for ref number: "
    //           << filledOrder->orderReferenceNumber << ", skipping trade"
    //           << std::endl;
    return;
  }

  OrderEntry &orderEntry = d_orders[filledOrder->orderReferenceNumber];

  //   std::cout << "Order executed: " << d_ticker << "  "
  //         << orderEntry.order->getOrderReferenceNumber() << " "
  //         << orderEntry.order->getPrice()
  //         << " Outstanding quantity: " << orderEntry.order->getQuantity()
  //         << ", Quantity executed: " << ntohl(filledOrder->executedShares)
  //         << std::endl;

  orderEntry.order->fill(ntohl(filledOrder->executedShares));
  updateLevelData(orderEntry.order->getSide(), orderEntry.order->getPrice(),
                  -orderEntry.order->getRemainingQuantity());
  if (orderEntry.order->filledOrCancelled()) {
    // std::cout << "Removing Order " <<
    // orderEntry.order->getOrderReferenceNumber() << std::endl;
    removeOrder(orderEntry);
  }

  // Strategy engine
  d_strategyEngine->applyOrderEvent(computeImbalance(), topBidPrice(),
                                    topAskPrice());
}

void Orderbook::removeOrder(OrderEntry &orderEntry) {
  OrderPointer &order = orderEntry.order;

  if (order->getSide() == Side::BUY) {
    d_bids[order->getPrice()].erase(orderEntry.location_);
    if (d_bids[order->getPrice()].empty())
      d_bids.erase(order->getPrice());
  } else {
    d_asks[order->getPrice()].erase(orderEntry.location_);
    if (d_asks[order->getPrice()].empty())
      d_asks.erase(order->getPrice());
  }

  d_orders.erase(order->getOrderReferenceNumber());
  // Remove order from memory pool
  d_orderPool.deallocate(order);
}

void Orderbook::updateLevelData(Side side, double price, double quantity) {
  if (side == Side::BUY) {
    // level data update
    d_bidLevelData[price].totalQuantity += quantity;
  } else {
    // level data update
    d_askLevelData[price].totalQuantity += quantity;
  }
}

} // namespace HFTSystem