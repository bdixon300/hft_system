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
- memory pool system for orderbook's orders


IN PROGRESS
- packet parser (parse cancel/modify orders)
- orderbook (l3)


TODOs:

- strategy engine
- move logging to separate thread to prevent writes slowing down system
- order manager (OUTCH -> OUTBOUND)

Nice to do - 

- SPSC / SPMC for lock free IPC (between packet parser and orderbook??) ?? - may not be needed for smaller security sizes

Current Lateny level

- 3 microseconds from packet hitting nic to before orderbook

Low latency Optimizations

C++ stuff

- as few mem copies as possible
- no virtual functions (avoid vtable lookup overhead)
- minimal heap usage (including stl - used custom allocators for stl), and custom memory pools on startup
- lock free programming on multithreading
- constexpr + compile time optimizations

Hardware/runtime configuration stuff

- cpu thread pinning
- openonload / dpdk (solarflare NIC)
