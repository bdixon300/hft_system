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
#include "spsc.h"

int main() {
  std::cout << "Starting a market data feed handler" << std::endl;

  // Simulate exchange feed (change this to path of your PCAP file)
  HFTSystem::PcapParser pcapParser(
      "/Users/ben/Documents/random_programming/market_data_feed_handler/"
      "nasdaw-itch5-total-view-mold-udp-marketdata.pcap");
  HFTSystem::MarketDataServer exchangeServer(pcapParser);

  // asynchrnous launches of each market data transmission
  std::thread exchangeServerThread([&]() {
    exchangeServer.start();
    exchangeServer.transmitMarketData();
  });

  // In next phase of project, I will add this SPSC queue shared between the
  // market data client thread (producer) and the OUTCH based TCP client
  // (consumer). HFTSystem::SPSC<HFTSystem::OutboundOrder> orderQueue(1000);

  // Actual HFT system (UDP multicast client, market data parser, orderbooks,
  // strategy engine etc)
  std::unordered_map<HFTSystem::LocateCode, HFTSystem::Ticker> codeMapping = {
      {10161, "TSLA"}};

  HFTSystem::MarketDataClient client(codeMapping);

  // asynchronous launches of market data client
  std::thread exchangeClientThread([&]() {
    client.start();
    client.handleMarketData();
  });

  exchangeServerThread.join();
  exchangeClientThread.join();
}
