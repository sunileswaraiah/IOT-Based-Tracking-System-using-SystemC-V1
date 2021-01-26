#ifndef _TOP_H_
#define _TOP_H_

#include <systemc.h>
#include "tlm.h"
#include "busHeader.h"

#include "simpleBus.h"
#include "slave.h"
#include "master.h"
#include "uart.h"

template<class T>
SC_MODULE(top)
{
  public:
    /*-------------------------------------------------------------------------
     * Constructor
     *-----------------------------------------------------------------------*/
    SC_CTOR(top);
    /*-------------------------------------------------------------------------
     * Hierarchy
     *-----------------------------------------------------------------------*/
    simpleBus<1, 2>   mybus;
    slave*            slave1;
    slave*            slave2;
    master<T>*           master1;
  private:
    /*-------------------------------------------------------------------------
     * FIFO
     *-----------------------------------------------------------------------*/
    //sc_fifo<int>      uart1_mc_u;
    sc_fifo<T>      uart1_u_mc;
    sc_fifo<T>      uart2_u_mc;
    sc_fifo<T>      uart2_mc_u;
    uart<T>*             uart1;
};
#endif /* _TOP_H_ */
