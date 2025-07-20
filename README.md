market data feed handler
=========
Hobby project to build a low latency HFT market data feed handler.
This project holds a UDP multicast server for testing, that publishes market data messages using Nasdaq ITCH protocol. 
The client then listens to the packets and parses them for building an in memory orderbook.

[![Build Status]
    (https://travis-ci.org/bdixoin/orderbook.svg)]
    (https://travis-ci.org/bdixoin/orderbook)
