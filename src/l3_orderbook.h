#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>

namespace HFTSystem {

enum class Side { BUY, SELL };

enum class OrderType {
  MARKET,
  LIMIT,
  FILLORKILL, // implement this
  IOC         // implement this
};

using Price = double;
using OrderId = int;
using Quantity = unsigned int;

// order should be a class with fill method and
class Order {
public:
  Order(OrderId order, Price price, Side side, Quantity quantity)
      : m_orderId(order), m_price(price), m_side(side),
        m_initialQuantity(quantity), m_remainingQuantity(quantity),
        m_orderType(OrderType::LIMIT) {}

  // good for day, fill or kill, fill and kill
  Order(OrderId order, Price price, Side side, Quantity quantity,
        OrderType orderType)
      : m_orderId(order), m_price(price), m_side(side),
        m_initialQuantity(quantity), m_remainingQuantity(quantity),
        m_orderType(orderType) {}

  Order(OrderId order, Side side, Quantity quantity, OrderType orderType)
      : m_orderId(order), m_price(-1), m_side(side),
        m_initialQuantity(quantity), m_remainingQuantity(quantity),
        m_orderType(orderType) {}

  ~Order() {}

  bool Filled() { return m_remainingQuantity == 0; }

  void Fill(Quantity quantity) {
    if (quantity > m_remainingQuantity) {
      throw std::logic_error(
          "Cannot fill an order quantity larger than remaining quantity");
    }

    m_remainingQuantity -= quantity;
  }

  Price getPrice() const { return m_price; }

  OrderId getOrderId() const { return m_orderId; }

  Quantity getQuantity() const { return m_initialQuantity; }

  Quantity getRemainingQuantity() const { return m_remainingQuantity; }

  Side getSide() const { return m_side; }

  OrderType getOrderType() const { return m_orderType; }

  void setPrice(Price price) { m_price = price; }

private:
  const OrderId m_orderId;
  Price m_price;
  const Side m_side;
  const Quantity m_initialQuantity;
  Quantity m_remainingQuantity;
  const OrderType m_orderType;
};

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

struct OrderEntry {
  OrderPointer order{nullptr};
  OrderPointers::iterator location_;
};

struct TradeInfo {
  OrderId id;
  Price price;
  Quantity quantity;
};

struct Trade {
  TradeInfo bidTrade;
  TradeInfo askTrade;
};

struct LevelData {
  Quantity totalQuantity;
};

using Trades = std::vector<Trade>;

class Orderbook {
public:
  Orderbook() {}
  ~Orderbook() {}

  bool isOrderFullyFillable(const OrderPointer &order) {
    if (order->getSide() == Side::BUY) {
      return m_asks.count(order->getPrice()) &&
             order->getQuantity() ==
                 m_askLevelData[order->getPrice()].totalQuantity;
    }

    return m_bids.count(order->getPrice()) &&
           order->getQuantity() ==
               m_bidLevelData[order->getPrice()].totalQuantity;
  }

  Trades addOrder(const OrderPointer &order) {
    std::lock_guard orderLock{m_orderMutex};

    if (m_orders.count(order->getOrderId()))
      throw std::logic_error("Duplicate order id");

    if (order->getOrderType() == OrderType::MARKET) {
      if (order->getSide() == Side::BUY && !m_bids.empty()) {
        auto &[worstBid, _] = *m_bids.rbegin();
        order->setPrice(worstBid);
      } else if (order->getSide() == Side::SELL && !m_asks.empty()) {
        auto &[worstAsk, _] = *m_asks.rbegin();
        order->setPrice(worstAsk);
      } else {
        // market order cannot be filled
        return {};
      }
    }

    if (order->getOrderType() == OrderType::FILLORKILL &&
        !isOrderFullyFillable(order)) {
      // fill or kill order cannot be filled
      return {};
    }

    // Limit order cases
    if (order->getSide() == Side::BUY) {
      auto &bids = m_bids[order->getPrice()];
      bids.emplace_back(order);
      m_orders.insert(
          {order->getOrderId(), OrderEntry{order, std::prev(bids.end())}});
      m_bidLevelData[order->getPrice()].totalQuantity += order->getQuantity();
    } else {
      auto &asks = m_asks[order->getPrice()];
      asks.emplace_back(order);
      m_orders.insert(
          {order->getOrderId(), OrderEntry{order, std::prev(asks.end())}});
      m_askLevelData[order->getPrice()].totalQuantity += order->getQuantity();
    }
  }

  void cancelOrder(const OrderPointer &order) {
    std::lock_guard orderLock{m_orderMutex};

    if (m_orders.count(order->getOrderId()))
      throw std::logic_error("Order does not exist, unable to cancel");

    OrderEntry &orderEntry = m_orders[order->getOrderId()];

    if (order->getSide() == Side::BUY) {
      // level data update
      m_bidLevelData[order->getPrice()].totalQuantity -=
          order->getRemainingQuantity();

      m_bids[order->getPrice()].erase(orderEntry.location_);
      if (m_bids[order->getPrice()].empty())
        m_bids.erase(order->getPrice());
    } else {
      // level data update
      m_askLevelData[order->getPrice()].totalQuantity -=
          order->getRemainingQuantity();

      m_asks[order->getPrice()].erase(orderEntry.location_);
      if (m_asks[order->getPrice()].empty())
        m_asks.erase(order->getPrice());
    }

    m_orders.erase(order->getOrderId());
  }

private:
  // actual price time priority order maps
  std::map<Price, OrderPointers, std::greater<Price>> m_bids;
  std::map<Price, OrderPointers, std::less<Price>> m_asks;

  // aggregate info about each level / order information
  std::unordered_map<Price, LevelData> m_bidLevelData;
  std::unordered_map<Price, LevelData> m_askLevelData;

  // order tracker
  std::unordered_map<OrderId, OrderEntry>
      m_orders; // makes it more efficient to cancel orders

  // thread safety
  std::mutex m_orderMutex;
};

} // namespace HFTSystem

#endif