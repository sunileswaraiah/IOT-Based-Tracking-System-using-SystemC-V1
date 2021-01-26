#ifndef _UART_H_
#define _UART_H_

#include "systemc.h"
#include "busHeader.h"

template<class T>
SC_MODULE(uart)
{
  public:
    /*-------------------------------------------------------------------------
     * Ports
     *-----------------------------------------------------------------------*/
    sc_port<sc_fifo_out_if<T> > uartTX01;
    //sc_port<sc_fifo_in_if<int> >  uartRX01;
    sc_port<sc_fifo_out_if<T> > uartTX02;
    sc_port<sc_fifo_in_if<T> >  uartRX02;
    /*-------------------------------------------------------------------------
     * Constructor
     *-----------------------------------------------------------------------*/
    SC_CTOR(uart);
  private:
    /*-------------------------------------------------------------------------
     * Methods/Threads
     *-----------------------------------------------------------------------*/
    void initUart(void);
    void getGpsData_f(void);
};

#endif /* _UART_H_ */
