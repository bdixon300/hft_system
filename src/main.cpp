#include <vector>
#include <list>
#include <stdexcept>
#include <iostream>
#include <pcap.h>

#include "pcap_file_reader.h"

int main()
{
    std::cout << "Hello, starting a market data feed handler" << std::endl;

    // PCAP File reader
    std::cout << "Testing the PCAP file reader component" << std::endl;

    MarketDataFeedHandler::PcapParser pcapParser("/Users/ben/Documents/random_programming/market_data_feed_handler/uncompressed_pcap.pcap");

    const u_char* packet;
    pcap_pkthdr* header = nullptr;
    pcapParser.parseNextPacket(packet, header);

}
