#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <exception>
#include "orderBook.h"

using namespace std;

orderBook ob;

void placeBidOrders()
{
  //increase number of orders to test performance of application
  int num_orders=900;

  do 
  {
    ob.addBid("YZ", "INFY", 100);
    ob.addBid("RM", "INFY", 445);
    ob.addBid("XR", "TCS", 155);
    ob.addBid("MR", "SAP", 345);
    ob.addBid("RR", "LGP", 845);
    ob.addBid("TX", "GS", 100); 
    ob.addBid("XX", "FF", 100);
    ob.addBid("TX", "FF", 100);
    ob.addBid("MMM", "FF", 140);

    num_orders--;

  } while (num_orders);
}

void placeAskOrders()
{
  //increase number of orders to test performance of application
  int num_orders=800;

  do 
  {
    ob.addAsk("XY", "INFY", 345);
    ob.addAsk("FY", "INFY", 45);
    ob.addAsk("XY", "TCS", 145);
    ob.addAsk("MM", "INFY", 45);
    ob.addAsk("MR", "GS", 145);
    ob.addAsk("LL", "FF", 330);
    ob.addAsk("MMM", "LGP", 733);
    ob.addAsk("TT", "SAP", 445);
    ob.addAsk("MM", "LG", 433);
    ob.addAsk("EM", "GP", 333);

    num_orders--;

  } while (num_orders);
}

int main()
{
  try
  {
    std::thread t1(placeBidOrders);
    std::thread t2(placeAskOrders);

    t1.join();
    t2.join();

    if(ob.isOBookEmpty())
    {
      std::cout << "Order book is empty!!" << std::endl;
    }

    ob.displayOrderBook();

  }catch(std::exception &e)
  {
    cout << e.what() << endl;
  }catch(...)
  {
    cout << "Ask Bid trades got some exception" << endl;
  }

  return 0;
}
