//
// Created by Emma Ryla on 7/13/23.
//
#pragma once


#include <string>
#include "OrderBook.h"

class Candlestick {
public:
    std::string date;
    double open;
    double high;
    double low;
    double close;
    double volume;

    Candlestick(std::string _date, double _open, double _high, double _low, double _close, double _volume)
            : date(_date), open(_open), high(_high), low(_low), close(_close), volume(_volume){}

    static std::vector<Candlestick> computeCandlesticks(OrderBook &orderBook, const std::string &productType);

};