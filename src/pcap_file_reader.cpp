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
    d_packetCnt = 0;
}

PcapParser::~PcapParser()
{
    pcap_close(d_pcapHandle);
}

int PcapParser::parseNextPacket(const u_char** packet, pcap_pkthdr** header)
{
    if (pcap_next_ex(d_pcapHandle, header, packet) >= 0)
    {
        /* print pkt timestamp and pkt len */
        // std::cout << "packet header info: [" 
        //           << " header length: " << (*header)->len 
        //           << ", tv_sec: " << (*header)->ts.tv_sec 
        //           << ", tv_usec" << (*header)->ts.tv_usec << "]" 
        //           << " full len " << (*header)->caplen
        //           << std::endl;

        /* Print the packet */
        //std::cout << "Raw Packet: " << std::endl;
        // for (int i=1; i < static_cast<int>((*header)->caplen) + 1; i++)
        // {
        //     printf("%02x ", (unsigned char)(*packet)[i]);
        //     if ((i + 1) % 16 == 0) printf("\n");
        // }

        //printf("\n");
        
        //std::cout << std::endl;

        // packet successfully processed
        d_packetCnt++;

        return 0;
    }
    else
    {
        std::cout << "No more packets to process. Final packet count: " << d_packetCnt << std::endl;
        // no more packets to parse
        return -1;
    }
}

} // MarketDataFeedHandler