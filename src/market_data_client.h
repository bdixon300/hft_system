# ifndef MARKET_DATA_CLIENT_H
# define MARKET_DATA_CLIENT_H

#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include "pcap_file_reader.h"

namespace MarketDataFeedSimulator {

/**
 * This class will be responsible for
 * establishing a udp multicast socket,
 * to broadcast the contents of a pcap file (using pcap file reader)
 * to a dedicated multicast group, simulating market data activity from
 * an exchange. This will be ingested/processed by the market data feed handler)
*/

class MarketDataClient
{
    public:
        MarketDataClient();
        ~MarketDataClient();

        void start();
        void stop();

        void handleMarketData();

    private:
        int d_socketFd;
        struct sockaddr_in d_multicastAddr;
        struct ip_mreq d_mreq;

};

}

# endif