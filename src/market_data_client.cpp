#include "market_data_client.h"
#include "market_data_server.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <chrono>

namespace HFTSystem {

MarketDataClient::MarketDataClient() {}

MarketDataClient::~MarketDataClient() { stop(); }

// 20 byte header for mold udp 64
#pragma pack(push, 1)
struct MoldUDP64Header {
  char session[10];  // ASCII
  uint64_t sequence; // network byte order
  uint16_t msgCount; // network byte order
};
#pragma pack(pop)

uint64_t custom_ntohll(uint64_t val) {
  uint32_t high_part = ntohl((uint32_t)(val >> 32));
  uint32_t low_part = ntohl((uint32_t)(val & 0xFFFFFFFFULL));
  return ((uint64_t)low_part << 32) | high_part;
}

void MarketDataClient::start() {
  std::cout << std::this_thread::get_id() << " Starting Market Data Client..."
            << std::endl;

  // 1. Create UDP socket
  if ((d_socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    throw std::runtime_error("socket creation failed");
  }

  // 2. Allow multiple sockets to reuse the same port
  int reuse = 1;
  if (setsockopt(d_socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
  }

  // 3. Bind to local port (must match multicast port)
  memset(&d_multicastAddr, 0, sizeof(d_multicastAddr));
  d_multicastAddr.sin_family = AF_INET;
  d_multicastAddr.sin_port = htons(PORT);
  d_multicastAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(d_socketFd, (struct sockaddr *)&d_multicastAddr,
           sizeof(d_multicastAddr)) < 0) {
    throw std::runtime_error("bind failed");
  }

  // 4. Join the multicast group
  d_mreq.imr_multiaddr.s_addr =
      inet_addr(IP_MULTICAST_GROUP);               // multicast group
  d_mreq.imr_interface.s_addr = htonl(INADDR_ANY); // default interface

  if (setsockopt(d_socketFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&d_mreq,
                 sizeof(d_mreq)) < 0) {
    throw std::runtime_error("setsockopt(IP_ADD_MEMBERSHIP) failed");
  }

  std::cout << std::this_thread::get_id()
            << " Joined multicast group: " << IP_MULTICAST_GROUP << std::endl;
}

void MarketDataClient::stop() {
  if (setsockopt(d_socketFd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&d_mreq,
                 sizeof(d_mreq)) < 0) {
    throw std::runtime_error("setsockopt(IP_DROP_MEMBERSHIP) failed");
  }

  std::cout << std::this_thread::get_id()
            << " Left multicast group: " << IP_MULTICAST_GROUP << std::endl;

  close(d_socketFd);

  std::cout << std::this_thread::get_id() << " Market Data Client stopped"
            << std::endl;
}

void MarketDataClient::handleMarketData() {
  std::cout << "Starting to ingest market data..." << std::endl;

  char buffer[1024];

  long long total = 0;
  long long packetNum = 0;

  while (true) {
    // To measure tick to trade latency
    auto start = std::chrono::high_resolution_clock::now();

    socklen_t addr_len = sizeof(d_multicastAddr);
    int n = recvfrom(d_socketFd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&d_multicastAddr, &addr_len);
    if (n < 0) {
      perror("recvfrom failed"); // FIXME: exceptions are bad
      break;
    }

    // const MoldUDP64Header *hdr =
    //     reinterpret_cast<const MoldUDP64Header *>(buffer);

    // std::cout << std::this_thread::get_id()
    //           << " Received Market Data Packet for parsing " << std::endl;

    // std::cout << "Session: " << hdr->session << std::endl;
    // std::cout << "Sequence: " << custom_ntohll(hdr->sequence) << std::endl;
    // std::cout << "Message count: " << ntohs(hdr->msgCount) << std::endl;

    int remainingBytes = n - sizeof(MoldUDP64Header);
    char *ptr = buffer + sizeof(MoldUDP64Header);

    while (remainingBytes > 2) {
      uint16_t msgLen = ntohs(*(uint16_t *)ptr);

      ptr += 2;
      remainingBytes -= 2;

      // Parse payload
      d_parser.parseMarketDataMessage(ptr);

      ptr += msgLen;
      remainingBytes -= msgLen;
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Calculate elapsed time in microseconds
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    total += duration;
    packetNum++;

    std::cout << "Elapsed time: " << duration << " microseconds" << std::endl;
    std::cout << "average: " << total / packetNum << " microseconds" << std::endl;
    std::cout << "packet num " << packetNum << std::endl;
  }
}

} // namespace HFTSystem