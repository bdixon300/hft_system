market data feed handler
=========
Hobby project to build a low latency HFT market data feed handler.
This project holds a UDP multicast server for testing, that publishes market data messages using Nasdaq ITCH protocol. 
The client then listens to the packets and parses them for building an in memory orderbook.

[![Build Status]
    (https://travis-ci.org/bdixoin/orderbook.svg)]
    (https://travis-ci.org/bdixoin/orderbook)

Done:
- UDP multicast server
- PCAP File parser
- UDP multicast client

IN PROGRESS
- packet parser


TODOs:

- orderbook (l3)
- strategy engine
- move logging to separate thread to prevent writes slowing down system
- order manager (OUTCH -> OUTBOUND)

Current Lateny level

- 3 microseconds from packet hitting nic to before orderbook

Low latency Optimizations

C++ stuff

- as few mem copies as possible
- no virtual functions (avoid vtable lookup overhead)
- minimal heap usage (including stl - used custom allocators for stl), or custom memory pools on startup
- lock free programming on multithreading
- constexpr + compile time optimizations

Hardware/runtime configuraiton stuff

- cpu thread pinning
- openonload / dpdk (solarflare NIC)
