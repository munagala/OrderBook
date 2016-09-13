#include <iostream>
#include <string>
#include <algorithm>
#include "traderDetails.h"

using namespace std;

void traderDetails::updateTraderStocks(string stock, int quantity)
{
  m_trader_stocks[stock] += quantity;
}

bool traderDetails::matchTrades(string stock, int &quantity, std::stringstream &ss, bool &match_trades)
{
  bool filled = false;
  auto stock_qunt = m_trader_stocks[stock];
  if(quantity > stock_qunt)
  {
    m_trader_stocks.erase(stock);
    quantity -= stock_qunt;
    ss  << m_trader << " trader " << stock  << " stocks " << "got filled." ;
    filled = false;
    match_trades = true;
  }else
  {
    m_trader_stocks[stock] -= quantity;
    quantity = 0;
    filled = true;
  }

  return filled;
}

void traderDetails::emptyTraderStocks(string stock)
{
  m_trader_stocks.erase(stock);
}

std::ostream& operator<<(std::ostream &os, const traderDetails& td)
{
  if(td.m_trader_stocks.empty()) 
  {
    //os << "Trader details are empty!!" << endl;
    return os;
  }

  for_each(td.m_trader_stocks.begin(), td.m_trader_stocks.end(), [&] (pair<string, int> _p)
  {
    if ( _p.second)
      cout  << td.m_trader  << "\t" << _p.first << "\t" << td.m_side << " \t " << _p.second << endl; 
  });

  return os;
}

void traderDetails::displayTraderDetails()
{
  cout << *this ;
}
