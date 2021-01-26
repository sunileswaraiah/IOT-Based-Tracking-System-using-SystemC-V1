#include "slave.h"

/*-----------------------------------------------------------------------------
 * Constructor
 *---------------------------------------------------------------------------*/
slave::slave(sc_module_name nm)
: sc_module            (nm)               // init module name
{
  cout << "ConstructorCPP - Slave " << name() << endl;

  slaveSocket.register_b_transport(this, &slave::b_transport);

  /* Initialize memory with random data */
  for (int i = 0; i < MEM_SIZE; i++)
      slave_mem[i] = 0xAA000000 | (rand() % 256);
}

/*-----------------------------------------------------------------------------
 * Methods/Threads/Functions
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Function Name: b_transport
 * Description  : Function receives data and store in the memory
 *              : are sends the data stored in memory
 * Parameters   : Generic payload, sc_time
 * Return Value : void
 *---------------------------------------------------------------------------*/
void slave::b_transport(tlm::tlm_generic_payload &trans,sc_time &delay)
{
  tlm::tlm_command cmd = trans.get_command();
  sc_dt::uint64    adr = trans.get_address() / ADDR_INCR;
  unsigned char*   ptr = trans.get_data_ptr();
  unsigned int     len = trans.get_data_length();
  unsigned char*   byt = trans.get_byte_enable_ptr();
  unsigned int     wid = trans.get_streaming_width();
#ifdef SLAVE_LOG
  char temp = 0;
#endif /* SLAVE_LOG */

  /* Error check in Address range */
  if (adr >= uint64(MEM_SIZE/ADDR_INCR))
  {
    trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    return;
  }
  /* Error check in byte enable */
  if (byt != 0)
  {
    trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
    return;
  }
  /*  Burst error check */
  if (len > 4 || wid < len)
  {
    trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
    return;
  }

  
  /* Read and write */
  if(cmd == tlm::TLM_READ_COMMAND)
  {
#ifdef SLAVE_LOG
    cout << "Slave  -   Read :  " << " @ " << sc_time_stamp()
      << ", address " << hex << adr
      << endl;
#endif /* SLAVE_LOG */
    memcpy(ptr, &slave_mem[adr], len);
  }
  else if (cmd == tlm::TLM_WRITE_COMMAND)
  {
#ifdef SLAVE_LOG
    temp  = *(reinterpret_cast<unsigned int*>(ptr));
    cout << "Slave  -   Write :  " << " @ " << sc_time_stamp()
      << ", address " << hex << adr
      << ", data " << dec << temp 
      << endl;
#endif /* SLAVE_LOG */
    memcpy(&slave_mem[adr], ptr, len);
  }
  else if (cmd == tlm::TLM_IGNORE_COMMAND)
  {
#ifdef SLAVE_LOG
    cout << "Slave  - IGNOR  :  " << " @ " << sc_time_stamp()
      << ", address " << hex << adr
      << ", cmd " << dec << cmd
      << endl;
#endif /* SLAVE_LOG */
    delay = SC_ZERO_TIME;
  }

  /* send response to the request */
  trans.set_response_status(tlm::TLM_OK_RESPONSE);
}

