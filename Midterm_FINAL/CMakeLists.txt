cmake_minimum_required(VERSION 3.25)
project(Midterm_testing)

set(CMAKE_CXX_STANDARD 17)

add_executable(Midterm_testing main.cpp MerkelMain.h MerkelMain.cpp OrderBookEntry.cpp OrderBookEntry.h OrderBook.cpp OrderBook.h CSVReader.cpp CSVReader.h Wallet.h Wallet.cpp Candlestick.h Candlestick.cpp CandlestickGraph.cpp CandlestickGraph.h)
