#include "market_data_client.h"
#include "market_data_server.h"

#include <iostream>
#include <thread>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h> 


namespace MarketDataFeedSimulator {

MarketDataClient::MarketDataClient() {}

MarketDataClient::~MarketDataClient()
{
    stop();
}

void MarketDataClient::start()
{
    std::cout << std::this_thread::get_id() << " Starting Market Data Client..." << std::endl;

    // 1. Create UDP socket
    if ((d_socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw std::runtime_error("socket creation failed");
    }

    // 2. Allow multiple sockets to reuse the same port
    int reuse = 1;
    if (setsockopt(d_socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
    }

    // 3. Bind to local port (must match multicast port)
    memset(&d_multicastAddr, 0, sizeof(d_multicastAddr));
    d_multicastAddr.sin_family = AF_INET;
    d_multicastAddr.sin_port = htons(PORT);
    d_multicastAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(d_socketFd, (struct sockaddr*)&d_multicastAddr, sizeof(d_multicastAddr)) < 0) {
        throw std::runtime_error("bind failed");
    }

    // 4. Join the multicast group
    d_mreq.imr_multiaddr.s_addr = inet_addr(IP_MULTICAST_GROUP);  // multicast group
    d_mreq.imr_interface.s_addr = htonl(INADDR_ANY);         // default interface

    if (setsockopt(d_socketFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&d_mreq, sizeof(d_mreq)) < 0) {
        throw std::runtime_error("setsockopt(IP_ADD_MEMBERSHIP) failed");
    }

    std::cout << std::this_thread::get_id() << " Joined multicast group: " << IP_MULTICAST_GROUP << std::endl;
}

void MarketDataClient::stop()
{
    if (setsockopt(d_socketFd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&d_mreq, sizeof(d_mreq)) < 0) {
        throw std::runtime_error("setsockopt(IP_DROP_MEMBERSHIP) failed");
    }

    std::cout << std::this_thread::get_id() << " Left multicast group: " << IP_MULTICAST_GROUP << std::endl;

    close(d_socketFd); 

    std::cout << std::this_thread::get_id() << " Market Data Client stopped" << std::endl;

}

void MarketDataClient::handleMarketData()
{
    std::cout << "Starting to ingest market data..." << std::endl;

    char buffer[1024];

    while (true) {
        socklen_t addr_len = sizeof(d_multicastAddr);
        int n = recvfrom(d_socketFd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr*)&d_multicastAddr, &addr_len);
        if (n < 0) {
            perror("recvfrom failed");
            break;
        }

        std::cout << std::this_thread::get_id() << " Received " << n << " bytes:" << std::endl;
        for (int i = 0; i < n; i++) {
            printf("%02x ", (unsigned char)buffer[i]);
            if ((i + 1) % 16 == 0) printf("\n");
        }
        printf("\n");
    }
}

}