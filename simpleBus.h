#ifndef _SIMPLE_BUS_H_
#define _SIMPLE_BUS_H_

#include <systemc>
#include "tlm.h"
#include "busHeader.h"

#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

template <int numMaster, int numSlave>
class simpleBus : public sc_module
{
  public:
    /*-------------------------------------------------------------------------
     * Constants/Variables/Functions
     *-----------------------------------------------------------------------*/
    /* Types */
    typedef tlm::tlm_generic_payload       payLoad_t;
    typedef tlm_utils::simple_target_socket_tagged<simpleBus>    slaveSocket_t;
    typedef tlm_utils::simple_initiator_socket_tagged<simpleBus> masterSocket_t;
    /* Sockets */
    slaveSocket_t    busSlaveSocket[numMaster];
    masterSocket_t   busMasterSocket[numSlave];
    /*-------------------------------------------------------------------------
     * Constructor
     *-----------------------------------------------------------------------*/
    simpleBus(sc_module_name inst);
  private:
    /*-------------------------------------------------------------------------
     * Methods/Threads
     *-----------------------------------------------------------------------*/
    unsigned int decode_address(const uint64 address, uint64& masked_address);
    void initiatorBTransport(int SocketId, payLoad_t& trans, sc_time& t);
};
#endif /* _SIMPLE_BUS_H_ */
