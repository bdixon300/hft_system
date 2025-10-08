#include <iostream>
#include <list>
#include <pcap.h>
#include <stdexcept>
#include <thread>
#include <vector>

#include "market_data_client.h"
#include "market_data_server.h"
#include "parser.h"
#include "pcap_file_reader.h"

int main() {
  std::cout << "Starting a market data feed handler" << std::endl;

  // Simulate exchange feed
  HFTSystem::PcapParser pcapParser(
      "/Users/ben/Documents/random_programming/market_data_feed_handler/"
      "nasdaw-itch5-total-view-mold-udp-marketdata.pcap");
  HFTSystem::MarketDataServer exchangeServer(pcapParser);

  // asynchrnous launches of each market data transmission
  std::thread exchangeServerThread([&]() {
    exchangeServer.start();
    exchangeServer.transmitMarketData();
  });

  // Actual HFT system (UDP multicast client, market data parser, orderbooks,
  // strategy engine etc)
  std::unordered_map<HFTSystem::LocateCode, HFTSystem::Ticker> codeMapping = {
      {10161, "TSLA"}, {10263, "UBER"}, {523, "AMZN"}};

  HFTSystem::MarketDataClient client(codeMapping);

  // asynchronous launches of market data client
  std::thread exchangeClientThread([&]() {
    client.start();
    client.handleMarketData();
  });

  exchangeServerThread.join();
  exchangeClientThread.join();
}
