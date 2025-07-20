# ifndef PCAP_FILE_READER_H
# define PCAP_FILE_READER_H

#include <fstream>
#include <pcap.h>

class PcapParser
{
public:
    PcapParser(const char* pcapFileName) {
        char errBuff[PCAP_ERRBUF_SIZE];
        d_pcapHandle = pcap_open_offline(pcapFileName, errBuff);
        if (!d_pcapHandle)
        {
            throw std::runtime_error("Unable to open PCAP file!");
        }
    }
    ~PcapParser()
    {
        pcap_close(d_pcapHandle);
    }

    /*
        This method parses the next available packet in the pcap handle
    */
    int parseNextPacket();

private:
    pcap_t* d_pcapHandle;
};


# endif