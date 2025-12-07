#include "orderbook.h"
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

  auto processOrderEvent = [&](auto &levelData) {
    d_orders.insert({order->getOrderReferenceNumber(), order});
    levelData[order->getPrice()].totalQuantity += order->getRemainingQuantity();
  };

  if (order->getSide() == Side::BUY) {
    processOrderEvent(d_bidLevelData);
  } else {
    processOrderEvent(d_askLevelData);
  }

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

  OrderPointer &order = d_orders[cancelOrder->orderReferenceNumber];

  //   std::cout << " Cancelling Order: " << d_ticker << "  "
  //             << orderEntry.order->getOrderReferenceNumber() << " "
  //             << orderEntry.order->getPrice() << " "
  //             << orderEntry.order->getQuantity() << std::endl;

  auto processCancel = [&](auto &levelData) {
    levelData[order->getPrice()].totalQuantity -= order->getRemainingQuantity();
  };

  if (Side::BUY == order->getSide()) {
    processCancel(d_bidLevelData);
  } else {
    processCancel(d_askLevelData);
  }

  removeOrder(order);

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

  OrderPointer &order = d_orders[partialCancelOrder->orderReferenceNumber];

  //   std::cout << " Partially Cancelling Order: " << d_ticker << "  "
  //             << orderEntry.order->getOrderReferenceNumber() << " "
  //             << orderEntry.order->getPrice()
  //             << " Outstanding quantity: " << orderEntry.order->getQuantity()
  //             << ", Quantity to cancel: " << partialCancelOrder->numShares
  //             << std::endl;

  order->partialCancel(ntohl(partialCancelOrder->numShares));

  auto processPartialCancel = [&](auto &levelData) {
    levelData[order->getPrice()].totalQuantity -=
        ntohl(partialCancelOrder->numShares);
  };

  if (Side::BUY == order->getSide()) {
    processPartialCancel(d_bidLevelData);
  } else {
    processPartialCancel(d_askLevelData);
  }

  // If order is fully cancelled remove from orderbook
  if (order->filledOrCancelled()) {
    removeOrder(order);
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

  OrderPointer &order = d_orders[filledOrder->orderReferenceNumber];

  //   std::cout << "Order executed: " << d_ticker << "  "
  //         << orderEntry.order->getOrderReferenceNumber() << " "
  //         << orderEntry.order->getPrice()
  //         << " Outstanding quantity: " << orderEntry.order->getQuantity()
  //         << ", Quantity executed: " << ntohl(filledOrder->executedShares)
  //         << std::endl;

  order->fill(ntohl(filledOrder->executedShares));

  auto processFill = [&](auto &levelData) {
    levelData[order->getPrice()].totalQuantity -= order->getRemainingQuantity();
  };

  if (Side::BUY == order->getSide()) {
    processFill(d_bidLevelData);
  } else {
    processFill(d_askLevelData);
  }

  if (order->filledOrCancelled()) {
    // std::cout << "Removing Order " <<
    // orderEntry.order->getOrderReferenceNumber() << std::endl;
    removeOrder(order);
  }

  // Strategy engine
  d_strategyEngine->applyOrderEvent(computeImbalance(), topBidPrice(),
                                    topAskPrice());
}

void Orderbook::removeOrder(OrderPointer order) {
  d_orders.erase(order->getOrderReferenceNumber());
  // Remove order from memory pool
  d_orderPool.deallocate(order);
}

} // namespace HFTSystem