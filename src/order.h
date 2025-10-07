#ifndef ORDER_H
#define ORDER_H

#include <list>
#include <memory>
#include <vector>

namespace HFTSystem {

enum class Side { BUY, SELL };

enum class OrderType {
  LIMIT, // NasDaq ITCH Total view 5 is for limit orders only i think
};

using Price = double;
using OrderId = int;
using Quantity = unsigned int;

/*
    Order types currently supported (not exhaustive)
*/
using TrackingNumber = uint16_t;
using LocateCode = uint16_t;
using OrderReferenceNumber = uint64_t;
using Ticker = std::string;

#pragma pack(push, 1)
struct AddOrder {
  LocateCode locateCode;
  TrackingNumber trackingNumber;
  char timestamp[6];
  OrderReferenceNumber orderReferenceNumber;
  uint8_t buySellIndicator;
  uint32_t numShares;
  char stock[8];
  uint32_t price;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PartialCancelOrder {
  LocateCode locateCode;
  TrackingNumber trackingNumber;
  char timestamp[6];
  OrderReferenceNumber orderReferenceNumber;
  uint32_t numShares;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CancelOrder {
  LocateCode locateCode;
  TrackingNumber trackingNumber;
  char timestamp[6];
  OrderReferenceNumber orderReferenceNumber;
};
#pragma pack(pop)

// Order class
class Order {
public:
  Order(OrderId order, Price price, Side side, Quantity quantity)
      : m_orderId(order), m_price(price), m_side(side),
        m_initialQuantity(quantity), m_remainingQuantity(quantity),
        m_orderType(OrderType::LIMIT) {}

  ~Order() {}

  bool filled();
  void fill(Quantity quantity);

  Price getPrice() const;
  OrderId getOrderId() const;
  Quantity getQuantity() const;
  Quantity getRemainingQuantity() const;
  Side getSide() const;
  OrderType getOrderType() const;
  void setPrice(Price price);

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