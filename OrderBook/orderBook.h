#pragma once 
#include <iostream>
#include <mutex>
#include <map>
#include <set>
#include <string>
#include "traderDetails.h"
#include "tradeLog.h"

using namespace std;

class orderBook {

    std::map<string, int> m_bid_stocks, m_ask_stocks;
    std::map<string, set<string>> m_bs_traders, m_as_traders;
    std::map<string, traderDetails*> m_bid_traders, m_ask_traders;
    tradeLog *TR_LOG = tradeLog::getInstance();
    std::mutex m_mu;

    void add(string trader, string stock, int qunt, bool bid);
    void filterAndMatchBidsBucket(string trader, string stock, int quantity);
    void filterAndMatchAsksBucket(string trader, string stock, int quantity);

  public:
    
    inline bool isOBookEmpty() const {
      return m_bid_stocks.empty() && m_ask_stocks.empty();
    }

    void addBid(string trader, string stock, int qunt);
    void addAsk(string trader, string stock, int qunt);
    void matchBidTraderStocks(string stock, string trader, int &qunt, bool &stocks_matched, bool &ask_stocks_matched);
    void matchAskTraderStocks(string stock, string trader, int &qunt, bool &stocks_matched, bool &bid_stocks_matched);

    //friend std::ostream& operator<<(std::ostream& os, const orderBook& ob);
    void displayOrderBook();
};


