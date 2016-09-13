#include <iostream>
#include <algorithm>
#include "orderBook.h"
#include "traderDetails.h"

using namespace std;

void orderBook::add( string trader, string stock, int qunt, bool bid)
{
  std::lock_guard<mutex> lock(m_mu);
  if(bid) 
  {
    //process bid order
    auto itr = m_bid_traders.find(trader);
    if(itr != m_bid_traders.end())
    {
      itr->second->updateTraderStocks(stock, qunt);  
    }else
    {
      traderDetails *tr = new traderDetails(trader, stock, "B", qunt);
      m_bid_traders.insert(std::make_pair(trader, tr));
    }

    filterAndMatchBidsBucket(trader, stock, qunt);
  }
  else 
  {
    //process ask order
    auto itr = m_ask_traders.find(trader);
    if(itr != m_ask_traders.end())
    {
      itr->second->updateTraderStocks(stock, qunt);  
    }else
    {
      traderDetails *tr = new traderDetails(trader, stock, "S", qunt);
      m_ask_traders.insert(std::make_pair(trader, tr));
    }

    filterAndMatchAsksBucket(trader, stock, qunt);
  }
}

void orderBook::filterAndMatchBidsBucket(string trader, string stock, int qunt)
{
  auto s_itr = m_ask_stocks.find(stock);
  std::set<string> s_ref;
  int org_qunt = qunt;
  bool stocks_filled = false, ask_stocks_filled = false;
  s_ref = m_bs_traders[stock];

  //check ask trades are available 
  if( s_itr != m_ask_stocks.end() )
  {
    matchBidTraderStocks(stock, trader, qunt, stocks_filled, ask_stocks_filled);
    if( !stocks_filled )
    {
      s_ref.insert(trader);
      m_bid_stocks[stock] += qunt;
      m_bid_traders[trader]->updateTraderStocks(stock, (qunt-org_qunt));
      m_bs_traders[stock] = s_ref;
    }
    else
    {
      auto itr = m_bid_traders.find(trader);
      if(itr != m_bid_traders.end())
        itr->second->emptyTraderStocks(stock);

      if(qunt > m_ask_stocks[stock])
        m_ask_stocks.erase(stock);
    }

  }else
  {
    m_bid_stocks[stock] += qunt;
    s_ref.insert(trader);
    m_bs_traders[stock] = s_ref;
  }
  
}

void orderBook::filterAndMatchAsksBucket(string trader, string stock, int qunt)
{
  auto s_itr = m_bid_stocks.find(stock);
  std::set<string> s_ref;
  int org_qunt = qunt;
  bool stocks_filled = false, bid_stocks_filled = false;
  s_ref = m_as_traders[stock];

  //check ask stocks are available
  if( s_itr != m_bid_stocks.end() )
  {
    matchAskTraderStocks(stock, trader, qunt, stocks_filled, bid_stocks_filled);
    if( !stocks_filled )
    {
      s_ref.insert(trader);
      m_ask_stocks[stock] += qunt;
      m_ask_traders[trader]->updateTraderStocks(stock, (qunt-org_qunt));
      m_as_traders[stock] = s_ref;
    }
    else
    {
      auto itr = m_ask_traders.find(trader);
      if(itr != m_ask_traders.end())
        itr->second->emptyTraderStocks(stock);

      if(qunt > m_bid_stocks[stock])
        m_bid_stocks.erase(stock);
    }

  }else
  {
    m_ask_stocks[stock] += qunt;
    s_ref.insert(trader);
    m_as_traders[stock] = s_ref; 
  }
}

void orderBook::matchBidTraderStocks(string stock, string trader, int &qunt, bool &ask_stks_matched, bool &bid_stks_matched)
{
  //get the ask traders related to stock
  auto tr_set = m_as_traders[stock];
  stringstream ss,_ss;

  for(auto itr = tr_set.rbegin(); itr != tr_set.rend(); itr++)
  {
    int ask_stocks = m_ask_traders[*itr]->getQuantity();
    if(m_ask_traders[*itr]->matchTrades(stock, qunt, _ss, bid_stks_matched))
    {
      ask_stks_matched = true;
      ss << trader << " trader " << stock << " stocks " << " got filled." ;

      if(qunt == 0)
        break;
    } else
    {
      if(bid_stks_matched)
      {
        m_ask_stocks[stock] -= ask_stocks;

        if(!_ss.str().empty())
        {
          TR_LOG->recordTradeMessage(_ss);
          _ss.str("");
        }
      }
    }
    bid_stks_matched = false;
  }

  if(!ss.str().empty())
  {
    TR_LOG->recordTradeMessage(ss);
    ss.str("");
  }
}

void orderBook::matchAskTraderStocks(string stock, string trader, int &qunt, bool &stocks_matched, bool &ask_stks_matched)
{
  //get the bid traders related to stock
  auto tr_set = m_bs_traders[stock];
  stringstream ss,_ss;

  for(auto itr = tr_set.rbegin(); itr != tr_set.rend(); itr++)
  {
    int bid_stocks = m_bid_traders[*itr]->getQuantity();
    if(m_bid_traders[*itr]->matchTrades(stock, qunt, _ss, ask_stks_matched))
    {
      stocks_matched = true;
      ss  << trader << " trader " << stock << " stocks " << " got filled." ;

      if(qunt == 0)
        break;
    } else
    {
      if(ask_stks_matched)
      {
        m_bid_stocks[stock] -= bid_stocks;
        if(!_ss.str().empty())
        {
          TR_LOG->recordTradeMessage(_ss); 
          _ss.str("");
        }
      }
    }
    ask_stks_matched = false;
  }

  if(!ss.str().empty())
  { 
    TR_LOG->recordTradeMessage(ss);
    ss.str("");
  }
}

void orderBook::addBid(string trader, string stock, int qunt)
{
  add(trader, stock, qunt, true);
}

void orderBook::addAsk(string trader, string stock, int qunt)
{
  add(trader, stock, qunt, false);
}
/*
std::ostream& operator<<(std::ostream& os, const orderBook& ob)
{
  if(ob.isOBookEmpty())
  {
    os << "Order Book is empty !!" << std::endl;
    return os; 
  }
  
  os << "TRADER" << "\t" << "STOCK" << "\t" << "QUANTITY" << "\t" << "SIDE" << endl;
  os << "------" << "\t" << "-----" << "\t" << "--------" << "\t" << "----" << endl;

  for_each(ob.m_ask_traders.rbegin(), ob.m_ask_traders.rend(), [&](std::pair<string, traderDetails*> _p) 
  {
    os << _p.first << "\t" << _p.second->getQuantity()
       << "\t\t " << "S" <<  std::endl; 
  });
  os << std::endl;

  for_each(ob.m_bid_traders.rbegin(), ob.m_bid_traders.rend(), [&](std::pair<string, traderDetails*> _p) 
  {
    os << _p.first << "\t" <<  _p.second->getQuantity()
       << "\t\t " << "B" <<  std::endl; 
  });

  return os;
}
*/
void orderBook::displayOrderBook() 
{
  cout << "TRADER" << "\t" << "STOCK" <<  "   " << "SIDE" << "\t" << "QUANTITY" << endl;
  cout << "------" << "\t" << "-----" <<  "   " << "----" << "\t" << "--------" << endl;

  for_each(m_ask_traders.rbegin(), m_ask_traders.rend(), [&](std::pair<string, traderDetails*> _p) 
  {
    _p.second->displayTraderDetails(); 
  });
  
  cout << endl;

  for_each(m_bid_traders.rbegin(), m_bid_traders.rend(), [&](std::pair<string, traderDetails*> _p) 
  {
    _p.second->displayTraderDetails(); 
  });
}
