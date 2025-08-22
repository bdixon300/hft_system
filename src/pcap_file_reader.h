# ifndef PCAP_FILE_READER_H
# define PCAP_FILE_READER_H

#include <fstream>
#include <pcap.h>

namespace MarketDataFeedHandler {

class PcapParser
{
public:
    PcapParser(const char* pcapFileName);
    ~PcapParser();

    /*
        This method parses the next available packet in the pcap handle
    */
    int parseNextPacket(const u_char* packet /* OUT */, pcap_pkthdr* header /* OUT */);

private:
    pcap_t* d_pcapHandle;
};

} // MarketDataFeedHandler

# endif