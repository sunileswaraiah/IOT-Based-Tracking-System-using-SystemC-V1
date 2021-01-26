#ifndef _SLAVE_H_
#define _SLAVE_H_

#include "busHeader.h"
#include "systemc.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

SC_MODULE(slave)
{
  public:
    /*-------------------------------------------------------------------------
     * Constructor
     *-----------------------------------------------------------------------*/
    SC_CTOR(slave);
    /*-------------------------------------------------------------------------
     * Socket
     *-----------------------------------------------------------------------*/
    tlm_utils::simple_target_socket<slave> slaveSocket;      /* slave socket */
  private:
    /*-------------------------------------------------------------------------
     * Methods
     *-----------------------------------------------------------------------*/
    virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay);
    /*-------------------------------------------------------------------------
     * varaiable
     *-----------------------------------------------------------------------*/
    unsigned int slave_mem[MEM_SIZE];
};
#endif /* _SLAVE_H_ */
