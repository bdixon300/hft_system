# market_data_feed_handler
# A rock solid project with enforced style, testing and static analysis
# Copyright 2015, Ben Dixon
cmake_minimum_required(VERSION 4.0.2)
project(market_data_feed_handler C CXX)
set(PROJECT_DESCRIPTION   "A rock solid project with enforced style, testing and static analysis")
set(PROJECT_AUTHOR        "Ben Dixon")
set(PROJECT_URL           "https://github.com/bdixon300/market_data_feed_handler")
set(PROJECT_VERSION_MAJOR "0")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "1")

# Build flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")


# ---------------------------------------------------------
# Features
# ---------------------------------------------------------
add_subdirectory(external)
add_subdirectory(src)

enable_testing()
add_subdirectory(test)
add_subdirectory(doc)
