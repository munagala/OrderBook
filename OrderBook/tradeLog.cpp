#include <iostream>
#include "tradeLog.h"

using namespace std;

tradeLog* tradeLog::trInstance = NULL;

tradeLog* tradeLog::getInstance()
{
  if(trInstance)
  {
    return trInstance;
  }
  else
  {
    trInstance = new tradeLog;
    return trInstance;
  }
}  

void tradeLog::recordTradeMessage(std::stringstream &os)
{
  std::lock_guard<std::mutex> locker(m_mu);

  if(m_trade_log.is_open())
  {
    string ss = os.str();
    m_trade_log << ss << endl;
  }
}

