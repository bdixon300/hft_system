#include "market_data_server.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

namespace MarketDataFeedSimulator {

MarketDataServer::MarketDataServer(PcapParser &pcapParser)
    : d_pcapParser(pcapParser) {}

MarketDataServer::~MarketDataServer() { stop(); }

void MarketDataServer::start() {
  std::cout << std::this_thread::get_id() << " Starting Exchange Server..."
            << std::endl;

  memset(&d_multicastAddr, 0, sizeof(d_multicastAddr));

  // Filling server information
  d_multicastAddr.sin_family = AF_INET; // IPv4
  d_multicastAddr.sin_port = htons(PORT);
  d_multicastAddr.sin_addr.s_addr = inet_addr(IP_MULTICAST_GROUP);

  // Create UDP socket
  if ((d_socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    throw std::runtime_error(
        "Socket Creation failed. Cannot run Market Data Server");
  }

  // Required as this program is running on same machine
  u_char loop = 1;
  if (setsockopt(d_socketFd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop,
                 sizeof(loop)) < 0) {
    throw std::runtime_error("setsockopt failed");
  }

  std::cout << std::this_thread::get_id() << " Sending market Data"
            << std::endl;
}

void MarketDataServer::stop() {
  close(d_socketFd);

  std::cout << std::this_thread::get_id() << " Exchange server stopped"
            << std::endl;
}

void MarketDataServer::transmitMarketData() {
  const u_char *packet;
  pcap_pkthdr *header = nullptr;

  while (d_pcapParser.parseNextPacket(&packet, &header) != -1) {
    sendto(d_socketFd, (const char *)(packet + HEADER_OFFSET),
           static_cast<int>(header->caplen) - HEADER_OFFSET, 0,
           (const struct sockaddr *)&d_multicastAddr, sizeof(d_multicastAddr));
  }
}

} // namespace MarketDataFeedSimulator