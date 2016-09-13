#pragma once
#include <iostream>
#include <string>
#include <map>
#include <set>
#include "tradeLog.h"

using namespace std;

class traderDetails 
{
  string m_trader;
  std::set<string> m_stocks;
  string m_side;
  int m_quantity;
  std::map<string, int> m_trader_stocks;
  tradeLog *TR_LOG; 

public:
  traderDetails():m_trader(""), m_side(""), m_quantity(0) {}  

  traderDetails(string _trader, string _stock, string _side, int _quantity):
             m_trader(_trader), m_side(_side), m_quantity(_quantity)
  {
    m_stocks.insert(_stock);
    updateTraderStocks(_stock,  _quantity);
    TR_LOG = tradeLog::getInstance();
  }  

  inline string getTrader() {
    return m_trader;
  }

  inline int getTraderStocksCount() {
    return m_trader_stocks.size();
  }

  inline int getQuantity() {
    return m_quantity;
  }

  void updateTraderStocks(string stock, int quantity);
  void displayTraderDetails();
  bool matchTrades(string stock, int &quantity, std::stringstream &ss, bool &match_trades);
  void emptyTraderStocks(string stock);

  friend std::ostream& operator<<(std::ostream &os, const traderDetails& td); 
};
