#include "top.h"

#include "simpleBus.cpp"
#include "slave.cpp"
#include "master.cpp"
#include "uart.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
 * Constructor
 *---------------------------------------------------------------------------*/
template<class T>
top<T>::top(sc_module_name nm)
  : sc_module(nm)
  , mybus("simpleBus")
//  , uart1_mc_u(8)
  , uart1_u_mc(8)
  , uart2_u_mc(8)
  , uart2_mc_u(8)
{
  cout << "ConstructorCPP - Top: " << name() << endl;

  slave1     = new slave("Memory-1");
  slave2     = new slave("Memory-2");
  master1    = new master<T>("MainMaster");
  uart1      = new uart<T>("UART");

  /*---------------------------------------------------------------------------
   * Connect blocks
   *-------------------------------------------------------------------------*/
  /*Connect the bus, bind TLM2 initiators to TLM2 target sockets on SimpleBus*/
  
  /* Connect master socket to bus slave socket */
  master1->masterSocket(mybus.busSlaveSocket[0]);

  /* Connect Bus master socket to memory slave sockets */
  mybus.busMasterSocket[0](slave1->slaveSocket);
  mybus.busMasterSocket[1](slave2->slaveSocket);

  /* Connect UART-1 to cpu or mc */
  master1->muartRX01(uart1_u_mc);
  uart1->uartTX01(uart1_u_mc);
  /* Connect UART-2 to cpu or mc */
  master1->muartRX02(uart2_u_mc);
  uart1->uartTX02(uart2_u_mc);

  /* Connect mc or cpu to UART */
  master1->muartTX02(uart2_mc_u);
  uart1->uartRX02(uart2_mc_u);
}
