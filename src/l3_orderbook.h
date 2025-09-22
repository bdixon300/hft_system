#ifndef ORDERBOOK_H
#define ORDERBOOK_H

namespace HFTSystem {

/**
 * This class will be responsible for maintaining an L3 orderbook
 * including aggregate stats for strategy engine to monitor for 
 * order generation
 *
 */
class Orderbook {
public:
  Orderbook();
  ~Orderbook();

  void addOrder();

private:
  // L3 orderbook
  // TODO
};

} // namespace HFTSystem

#endif