#pragma once
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class tradeLog 
{
  std::mutex m_mu;
  std::ofstream m_trade_log;
  static tradeLog *trInstance;

  tradeLog() 
  {
    m_trade_log.open("tradeLog.txt");
  }

  ~tradeLog()
  {
    m_trade_log.close();
  }

public:
  static tradeLog* getInstance();
  void recordTradeMessage(std::stringstream &os);
  inline void operator << ( std::stringstream &os)
  {
    m_trade_log << os;
  }

};


