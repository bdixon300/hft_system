# ifndef MARKET_DATA_SERVER_H
# define MARKET_DATA_SERVER_H

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

constexpr int PORT = 4003; // Common UDP multicast port

class MarketDataServer
{
    public:
        MarketDataServer(PcapParser& pcapParser);
        ~MarketDataServer();

        void start();
        void stop();

        void transmitMarketData();

    private:
        PcapParser d_pcapParser;
        int d_socketFd;
        struct sockaddr_in d_multicastAddr;

};

}

# endif