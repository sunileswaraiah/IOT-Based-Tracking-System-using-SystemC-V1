#ifndef _MASTER_H_
#define _MASTER_H_

#include "busHeader.h"
#include "systemc.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

template<class T>
SC_MODULE(master)
{
  public:
    /*-------------------------------------------------------------------------
     * Ports
     *-----------------------------------------------------------------------*/
    sc_port<sc_fifo_in_if<T> >      muartRX01;
    //sc_port<sc_fifo_out_if<int> >      muartTX01;
    sc_port<sc_fifo_in_if<T> >      muartRX02;
    sc_port<sc_fifo_out_if<T> >     muartTX02;
    /*-------------------------------------------------------------------------
     * Constructor
     *-----------------------------------------------------------------------*/
    SC_CTOR(master);
    /*-------------------------------------------------------------------------
     * Socket
     *-----------------------------------------------------------------------*/
    tlm_utils::simple_initiator_socket<master> masterSocket;
    /*-------------------------------------------------------------------------
     * Events
     *-----------------------------------------------------------------------*/
    sc_event read_gps;
    /*-------------------------------------------------------------------------
     * varaiables
     *-----------------------------------------------------------------------*/
    unsigned int slaveAddress;
  private:
    /*-------------------------------------------------------------------------
     * Methods/Threads
     *-----------------------------------------------------------------------*/
    void processData(uint32_t address, char* data, int cmd);
    void storeGPSData(void);
    void storeGSMData(void);
    void getGPSData(void);
};
#endif /*  _MASTER_H_ */
