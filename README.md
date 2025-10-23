HFT System
=========
Hobby project to build a low latency HFT system.
This project holds a UDP multicast server that parses a PCAP file from a real trading day on Nasdaq. This server acts as a mock simulation of a server from a real exchange, that publishes market data messages using Nasdaq ITCH protocol.

The remaining components make up a low latency HFT system. This includes a UDP multicast client that listens to network packets, parses
the ITCH messages and builds a set of in memory orderbooks from the messages. These orderbooks are monitored by the strategy engines
to discover trading oppportunities. When an opportunity is found the strategy engine sends orders out to the exchange server using the OUTCH
protocol.

[![Build Status]
    (https://travis-ci.org/bdixoin/orderbook.svg)]
    (https://travis-ci.org/bdixoin/orderbook)

Done:
- UDP multicast server
- PCAP File parser
- UDP multicast client
- memory pool system for orderbook's orders
- packet parser (parse cancel/modify orders)
- orderbook (l3)
- strategy engine


IN PROGRESS
- order manager (OUTCH -> OUTBOUND)


TODOs:

- use one shared order generator across orderbooks and use a spsc queue for the events
- move logging to separate thread to prevent writes slowing down system

Nice to do - 

- create custom map std:: that uses contiguous memory. std::map is a RB tree and memory is not contiguous (use this in orderbook implementation)


Current Mean Latency level (on Apple M4 chip when run locally)

- 3-4 microseconds from packet getting from NIC to OUTCH message sent

Current Median / Tail latency levels (on Apple M4 chep when running locally)

- 6 - 10 microseconds

Low latency Optimizations

C++ stuff

- as few mem copies as possible
- no virtual functions (avoid vtable lookup overhead)
- minimal heap allocation/deallocation usage when live (including stl - used custom allocators for stl), and custom memory pools on startup
- lock free programming on multithreading (SPSC queue)
- constexpr + compile time optimizations
- TMP optimizations

Hardware/runtime configuration stuff

- cpu thread pinning / overclocking
- openonload / dpdk (solarflare NIC) for kernel bypass (cannot be done on my local machine)
