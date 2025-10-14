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

IN PROGRESS
- orderbook (l3)
- strategy engine


TODOs:

- move logging to separate thread to prevent writes slowing down system
- order manager (OUTCH -> OUTBOUND)

Nice to do - 

- create custom map std:: that uses contiguous memory. std::map is a RB tree and memory is not contiguous (use this in orderbook implementation)
- SPSC / SPMC for lock free IPC (between packet parser and orderbook??) ?? - may not be needed for smaller security sizes

Current Lateny level (on Apple M4 chip when run locally)

- 3-4 microseconds from packet hitting nic to OUTCH message sent

Low latency Optimizations

C++ stuff

- as few mem copies as possible
- no virtual functions (avoid vtable lookup overhead)
- minimal heap usage (including stl - used custom allocators for stl), and custom memory pools on startup
- lock free programming on multithreading
- constexpr + compile time optimizations
- TMP optimizations

Hardware/runtime configuration stuff

- cpu thread pinning / overclocking
- openonload / dpdk (solarflare NIC)
