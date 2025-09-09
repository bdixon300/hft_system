#include "pcap_file_reader.h"

#include <stdio.h>
#include <pcap/pcap.h>
#include <iostream>

namespace MarketDataFeedSimulator {

PcapParser::PcapParser(const char* pcapFileName) {
    char errBuff[PCAP_ERRBUF_SIZE];
    d_pcapHandle = pcap_open_offline(pcapFileName, errBuff);
    if (!d_pcapHandle)
    {
        throw std::runtime_error("Unable to open PCAP file!");
    }
}

PcapParser::~PcapParser()
{
    pcap_close(d_pcapHandle);
}

int PcapParser::parseNextPacket(const u_char* packet, pcap_pkthdr* header)
{
    if (pcap_next_ex(d_pcapHandle, &header, &packet) >= 0)
    {
        /* print pkt timestamp and pkt len */
        std::cout << "packet header info: [" 
                  << " header length: " << header->len 
                  << ", tv_sec: " << header->ts.tv_sec 
                  << ", tv_usec" << header->ts.tv_usec << "]" 
                  << std::endl;

        /* Print the packet */
        for (int i=1; i < static_cast<int>(header->caplen) + 1; i++)
        {
            std::cout << packet[i-1];
            if ( (i % 16) == 0) std::cout << std::endl;
        }
        
        std::cout << std::endl;

        // packet successfully processed
        return 0;
    }
    else
    {
        // no more packets to parse
        return -1;
    }
}

} // MarketDataFeedSimulator