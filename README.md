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
- basic order generator


TODOs (next items)

- Build a TCP client for submiting Orders using the OUTCH protocol
- use one shared order generator across multiple orderbooks/strategy engines and use a spsc queue (between the ITCH parser/orderbook/strategy engine) and the OUTCH client for submitting orders.

- move logging to separate thread to prevent writes slowing down system

Nice to do (Potential further optimizations) - 

- create custom std::map that uses contiguous memory. std::map is a RB tree and memory is not contiguous (use this in orderbook implementation to reduce latency).

- look for further optimization possibilities

- Implement more sophisticated HFT strategies 

Current Mean Latency level (on Apple M4 chip when run locally)

- 3-4 microseconds from packet getting from NIC to OUTCH message sent

Current Median / Tail latency levels (on Apple M4 chep when running locally)

- 6 - 10 microseconds

Low latency Optimization Principles

- Made as few memory copies as possible
- no virtual functions (avoid vtable lookup overhead)
- minimal heap allocation/deallocation usage when live, ie: used custom memory pool on startup
for orders in the orderbook.
- lock free programming on multithreading side (SPSC queue)
- constexpr + compile time optimizations
- TMP optimizations

Hardware/runtime configuration stuff

- cpu thread pinning / overclocking
- openonload / dpdk (solarflare NIC) for kernel bypass (cannot be done on my local machine)
