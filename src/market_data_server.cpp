#include "market_data_server.h"

#include <iostream>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h> 


namespace MarketDataFeedSimulator {

MarketDataServer::MarketDataServer(PcapParser& pcapParser): d_pcapParser(pcapParser) {}

void MarketDataServer::start()
{
    memset(&d_multicastAddr, 0, sizeof(d_multicastAddr)); 

    // Filling server information 
    d_multicastAddr.sin_family = AF_INET; // IPv4 
    d_multicastAddr.sin_port = htons(PORT); 
    inet_pton(AF_INET, IP_MULTICAST_GROUP, &d_multicastAddr.sin_addr);

    // Create UDP socket
    if ((d_socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw std::runtime_error("Socket Creation failed. Cannot run Market Data Server");
    }

    std::cout << "Sending market Data" << std::endl;
}

void MarketDataServer::stop()
{
    close(d_socketFd); 
}

void MarketDataServer::transmitMarketData()
{
    const u_char* packet;
    pcap_pkthdr* header = nullptr;

    while (d_pcapParser.parseNextPacket(packet, header) != -1)
    {
        sendto(d_socketFd,
            (const char *)packet,
            strlen((const char *)packet),
            0,
            (const struct sockaddr *) &d_multicastAddr,  
            sizeof(d_multicastAddr)
        ); 
        
        std::cout<< "Packet Sent." << std::endl; 
        std::cout << "Parsing another packet" << std::endl;
    }
}

}