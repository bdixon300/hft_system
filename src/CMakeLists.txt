# Target:   orderbook_lib
# Type:     Library
file(GLOB SOURCE *.cpp)
add_library(market_data_feed_handler_lib SHARED ${SOURCE})
set_target_properties(market_data_feed_handler_lib PROPERTIES
    "OUTPUT_NAME" "market_data_feed_handler"
    "INCLUDE_DIRECTORIES" "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(market_data_feed_handler.tsk main.cpp)
target_link_libraries(market_data_feed_handler.tsk PRIVATE market_data_feed_handler_lib)


install(TARGETS market_data_feed_handler_lib EXPORT market_data_feed_handler DESTINATION ${CMAKE_SOURCE_DIR}/lib)
export(TARGETS market_data_feed_handler_lib FILE ${CMAKE_BINARY_DIR}/market_data_feed_handler-config.cmake)
