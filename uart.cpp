#include "uart.h"

/*-------------------------------------------------------------------------
 * Constructor
 *-----------------------------------------------------------------------*/
template<class T>
uart<T>::uart(sc_module_name nm)
  : sc_module(nm)
{
  SC_HAS_PROCESS(uart);
  cout << "ConstructorCPP - UART " << name() <<endl;
  SC_THREAD(initUart);
  SC_THREAD(getGpsData_f);
}


/*----------------------------------------------------------------------------
 * Methods/Threads
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Thread Name  : getGpsData_f
 * Description  : Thread reads GPS data from fifo and Displays on terminal
 * Parameters   : void
 * Return Value : void
 *---------------------------------------------------------------------------*/
template<class T>
void uart<T>::getGpsData_f(void)
{
  char data = 0;
  while(1)
  {
	  /* Read GPS data from fifo */
    wait(uartRX02->data_written_event());
    data = uartRX02->read();
    cout << "GPS data - Read : " << "  @ " << sc_time_stamp() 
        << ", data " << hex << data << endl;
    cout << " " << endl;
  }
}

/*-----------------------------------------------------------------------------
 * Thread Name  : initUart
 * Description  : Sends the GSM and GPS data to master through fifo
 * Parameters   : void
 * Return Value : void
 *---------------------------------------------------------------------------*/
template<class T>
void uart<T>::initUart(void)
{
  string uartData = "$GPGGA,181908.00,3404.7041778,N,07044.3966270,W,4,13,1.00,495.144,M,29.200,M,0.10,0000*40";
  string gsmData = "GSM DATA 123";
  char data = 0;

  /*----------------------------------------------------------------------
   * Write GPS data to slave-01
   *--------------------------------------------------------------------*/
  wait(2, SC_NS);
  cout << " " << endl;
  cout << "------------------------------------------------------" << endl;
  cout << "Write GPS DATA to SLAVE-01 " << endl;
  cout << "------------------------------------------------------" << endl;
  for(unsigned int i = 0; i < uartData.length(); i++)
  {
    uartTX01->write(uartData[i]);
    wait(1, SC_NS);
  }

  /*----------------------------------------------------------------------
   * Write GSM data to slave-02
   *--------------------------------------------------------------------*/
  wait(2, SC_NS);
  cout << " " << endl;
  cout << "------------------------------------------------------" << endl;
  cout << "Write GSM DATA to SLAVE-02 " << endl;
  cout << "------------------------------------------------------" << endl;
  for(unsigned int i = 0; i < gsmData.length(); i++)
  {
    uartTX02->write(gsmData[i]);
    wait(1, SC_NS);
  }

  /*----------------------------------------------------------------------
   * Read GPS data from slave-01
   *--------------------------------------------------------------------*/
  wait(50, SC_NS);
  cout << " " << endl;
  cout << "------------------------------------------------------" << endl;
  cout << "get GPS DATA from slave1 " << endl;
  cout << "------------------------------------------------------" << endl;
  data = 'x';
  uartTX02->write(data);

  wait(10, SC_SEC);
  cout << " " << endl;
  cout << "------------------------------------------------------" << endl;
  cout << "End of simulation " << endl;
  cout << "------------------------------------------------------" << endl;
  sc_stop();
}

