#include <vector>
#include <list>
#include <stdexcept>
#include <iostream>
#include <pcap.h>
#include <thread>


#include "pcap_file_reader.h"
#include "market_data_client.h"
#include "market_data_server.h"


int main()
{
    std::cout << "Hello, starting a market data feed handler" << std::endl;

    MarketDataFeedSimulator::PcapParser pcapParser("/Users/ben/Documents/random_programming/market_data_feed_handler/nasdaw-itch5-total-view-mold-udp-marketdata.pcap");
    MarketDataFeedSimulator::MarketDataServer exchangeServer(pcapParser);
    MarketDataFeedSimulator::MarketDataClient client;

    // asynchrnous launches of each market data transmission
    std::thread exchangeServerThread([&](){
        exchangeServer.start();
        exchangeServer.transmitMarketData();
    });

    // asynchronous launches of market data client
    std::thread exchangeClientThread([&](){
        client.start();
        client.handleMarketData();
    });

    exchangeServerThread.join();
    exchangeClientThread.join();
}
