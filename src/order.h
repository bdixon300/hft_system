#ifndef ORDER_H
#define ORDER_H

#include <list>
#include <memory>
#include <vector>

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

  bool Filled();
  void Fill(Quantity quantity);

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

using OrderPointer = Order *;
using OrderPointers = std::list<OrderPointer>;

using Orders = std::list<Order>;

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

} // namespace HFTSystem

#endif