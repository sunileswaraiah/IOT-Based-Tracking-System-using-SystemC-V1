#include "simpleBus.h"

/*-----------------------------------------------------------------------------
 * Constructor
 *---------------------------------------------------------------------------*/
template <int numMaster, int numSlave>
simpleBus<numMaster, numSlave>::simpleBus(sc_module_name inst)
  : sc_module(inst)
{
  cout << "ConstructorCPP - SimpleBus: " << name() << endl;
  /* Register slave sockets for all master
   * A master devices socket is connected to a bus slave  socket.
   */
  for (unsigned int i = 0; i < numMaster; i++)
  {
    busSlaveSocket[i].register_b_transport(this,
           &simpleBus::initiatorBTransport, i);
  }
}

/*-----------------------------------------------------------------------------
 * Methods
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Method_name  : decode_address
 * Description  : Get port ID and masked address from 32 bit address
 * Parameters   : address, masked address
 * Return Value : PortID of the slave
 *---------------------------------------------------------------------------*/
template <int numMaster, int numSlave>
unsigned int simpleBus<numMaster, numSlave>::decode_address(const uint64 address, uint64& masked_address)
{
  /* The upper 4 bits[31-28] are the ID of the bus contributors */
  unsigned int portID;
  portID = (unsigned int) (address >> SSADDRESS);
  /* The lower 28 bits[27-0] will be masked */
  masked_address = address & MSADDRESS;
  return portID;
}

/*-----------------------------------------------------------------------------
 * Method_name  : initiatorBTransport
 * Description  : Forward each call to the target/initiator
 * Parameters   : Socket ID, Generic payload, sc_time
 * Return Value : void 
 *---------------------------------------------------------------------------*/
template <int numMaster, int numSlave>
void simpleBus<numMaster, numSlave>
::initiatorBTransport(int SocketId, payLoad_t& trans, sc_time& t)
{
  masterSocket_t* initSocket;
  sc_dt::uint64 address =  trans.get_address();
  sc_dt::uint64 masked_address;
  /* portId will be used to find the correct slave */
  unsigned int portId = decode_address(address, masked_address);

  if (portId < numSlave)
  {
    /* The master socket of the bus (must be connected to a slave) */
    initSocket = &busMasterSocket[portId];
    /* Sets the address without the upper four bits, which are the index number
     * of the array of slave sockets.
     */
    trans.set_address(masked_address);

    (*initSocket)->b_transport(trans, t);

    cout << "Bus    - Transp :  " << " @ " << sc_time_stamp()
      << ", address " << hex << trans.get_address()
      << ", data "  << hex
      << *reinterpret_cast<unsigned char*>(trans.get_data_ptr())
      << ", PortID " << dec << portId
      << endl;
  }
  else
  {
    cout << RED << "Bus  - Error : Invalid slave address " << " @ " << sc_time_stamp()
    << ", address " << hex << trans.get_address()
    << ", PortID " << dec << portId
    << RESET << endl;
  }    
}

