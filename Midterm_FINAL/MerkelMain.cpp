#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Wallet.h"
#include "Candlestick.h"
#include "CandlestickGraph.h"


MerkelMain::MerkelMain()
{


}

void MerkelMain::init()
{
    //loadOrderBook();
    int input;

    currentTime = orderBook.getEarliestTime();
    wallet.insertCurrency("BTC", 10);

    while(true)
    {

       /* std::vector<Candlestick> candlesticks = Candlestick::computeCandlesticks(orderBook, "ETH/BTC,bid");
        for (size_t i = 0; i < 5; ++i) {
            std::cout << "Candlestick " << i + 1 << ":\n";
            std::cout << "Open: " << candlesticks[i].open << "\n";
            std::cout << "High: " << candlesticks[i].high << "\n";
            std::cout << "Low: " << candlesticks[i].low << "\n";
            std::cout << "Close: " << candlesticks[i].close << "\n";
            std::cout << "Time: " << candlesticks[i].date << "\n";
            std::cout << "--------------------------\n";
        }*/
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 continue
    std::cout << "6: Continue " << std::endl;
    // 7 get candlestick graph
    std::cout << "7: Candlesticks "<< std::endl;

    std::cout <<"8: Volume "<<std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;

}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;

    }
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                    tokens[1],
                    tokens[2],
                    currentTime,
                    tokens[0],
                    OrderBookType::ask
            );
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                    tokens[1],
                    tokens[2],
                    currentTime,
                    tokens[0],
                    OrderBookType::bid
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }

    }
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-8" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}
//written by me!
void MerkelMain::printCandlesticks(){
    std::string userInput = "";
    std::cout << "Please enter a 'product,ask/bid' to generate a candlestick graph for: " << std::endl;//take user input
    std::getline (std::cin,userInput);

    std::vector<Candlestick> candlesticks = Candlestick::computeCandlesticks(orderBook, userInput);//get candlesticks to this spec

    std::vector<Candlestick> candlesticksGroup(candlesticks.begin() + 1, candlesticks.begin() + 11);//ideally, return a random set (unable to rand in c++??)
    CandlestickGraph graph;
    graph.buildCandlestick(candlesticksGroup, userInput);//build a new graph for this group

}
//written by me!
void MerkelMain::printVolume(){
    std::string userInput = "";
    std::cout << "Please enter a 'product,ask/bid' to generate a 'price volume' graph for: " << std::endl;//take user input
    std::getline (std::cin,userInput);

    std::vector<Candlestick> candlesticks = Candlestick::computeCandlesticks(orderBook, userInput);//get candlesticks to this spec

    std::vector<Candlestick> candlesticksGroup(candlesticks.begin() + 1, candlesticks.begin() + 11);//ideally, return a random set
    CandlestickGraph graph;
    graph.volumeGraph(candlesticksGroup, userInput);//build a new graph for this group
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-7" << std::endl;
    }
    if (userOption == 1)
    {
        printHelp();
    }
    if (userOption == 2)
    {
        printMarketStats();
    }
    if (userOption == 3)
    {
        enterAsk();
    }
    if (userOption == 4)
    {
        enterBid();
    }
    if (userOption == 5)
    {
        printWallet();
    }
    if (userOption == 6)
    {
        gotoNextTimeframe();
    }
    if (userOption == 7)
    {
        printCandlesticks();
    }
    if (userOption == 8)
    {
        printVolume();
    }
}
