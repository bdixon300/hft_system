#ifndef MARKET_DATA_CLIENT_H
#define MARKET_DATA_CLIENT_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "pcap_file_reader.h"

namespace MarketDataFeedSimulator {

constexpr int MAX_BUFFER_SIZE = 1024;

/**
 * This class will be responsible for
 * establishing a udp multicast socket receiver,
 * ingesting/parsing packets from an exchange server simulator
 *
 */
class MarketDataClient {
public:
  MarketDataClient();
  ~MarketDataClient();

  void start();
  void stop();

  /* Run in busy wait to pull/parse network packets from NIC */
  void handleMarketData();

private:
  int d_socketFd;
  struct sockaddr_in d_multicastAddr;
  struct ip_mreq d_mreq;

  /** Helper to parse the raw market data payload */
  void parseMarketDataMessage(const char *payload, const unsigned int msgLen);
};

} // namespace MarketDataFeedSimulator

#endif