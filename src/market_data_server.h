#ifndef MARKET_DATA_SERVER_H
#define MARKET_DATA_SERVER_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "pcap_file_reader.h"

namespace HFTSystem {

/**
 * This class will be responsible for
 * establishing a udp multicast socket,
 * to broadcast the contents of a pcap file (using pcap file reader)
 * to a dedicated multicast group, simulating market data activity from
 * an exchange. This will be ingested/processed by the market data feed handler)
 */

// https://www.nasdaqtrader.com/Trader.aspx?id=FeedIPS_Other
constexpr int PORT = 55370;
constexpr char IP_MULTICAST_GROUP[] = "233.54.12.132";

// Offset to apply when extracting the data payload (skipping ethernet
// headers/udp headers/IP headers) 8 bytes (UDP/Transport) 20 bytes (IP4) 4
// BYTES (VLAN) 14 bytest (ethernet) 8 + 20 + 4 + 14 = 46
constexpr int HEADER_OFFSET = 46;

class MarketDataServer {
public:
  MarketDataServer(PcapParser &pcapParser);
  ~MarketDataServer();

  void start();
  void stop();

  void transmitMarketData();

private:
  PcapParser d_pcapParser;
  int d_socketFd;
  struct sockaddr_in d_multicastAddr;
};

} // namespace HFTSystem

#endif