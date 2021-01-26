#include "master.h"
#include <cstring>

/*----------------------------------------------------------------------------
 * Constructor
 *--------------------------------------------------------------------------*/
template<class T>
master<T>::master(sc_module_name nm)
  : sc_module(nm)
{
  SC_HAS_PROCESS(master);
  cout << "ConstructorCPP - Master " << name() <<  endl;
  SC_THREAD(storeGPSData);
  SC_THREAD(storeGSMData);
  SC_THREAD(getGPSData);
}


/*----------------------------------------------------------------------------
 * Methods/Threads/Functions
 *--------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Function Name: processData
 * Description  : Function will fill the pauload and transmit to the bus
 * Parameters   : Address, Data, Command
 * Return Value : void
 *---------------------------------------------------------------------------*/
template<class T>
void master<T>::processData(uint32_t address, char* data, int cmd)
{
  unsigned char s_data = 0;
  tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;
  sc_time delay = sc_time(SC_ZERO_TIME);

  s_data = *data;
  /* set write or read command */
  if (cmd == tlm::TLM_WRITE_COMMAND) {
    trans->set_command(tlm::TLM_WRITE_COMMAND);
  } else {
    trans->set_command(tlm::TLM_READ_COMMAND);
  }
    trans->set_address(address);
    trans->set_data_ptr(reinterpret_cast<unsigned char*>(&s_data));
    trans->set_data_length(4);
    /* data_length to indicate no streaming */
    trans->set_streaming_width(4);
    /* 0 indicates unused */
    trans->set_byte_enable_ptr(0);
    /* Mandatory initial value */
    trans->set_dmi_allowed(false);
    /* Mandatory initial value */
    trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    /* Blocking transport call */
    masterSocket->b_transport(*trans, delay);

    wait(cpuSleep);
    /* Initiator obliged to check response status */
    if (trans->is_response_error())
    {
      cout << RED <<"Response ERROR : " << trans->get_response_string().c_str()
        << RESET << endl;
      return;
    }

#ifdef MASTER_LOG
    if (cmd == tlm::TLM_WRITE_COMMAND) {
      cout << "Master -  Write :  " << " @ " << sc_time_stamp()
        << ", address "       << hex << address
        << ", data "       << hex << s_data
        << endl;
      cout << " " << endl;
    } else {
      cout << "Master -   Read :  " << " @ " << sc_time_stamp()
        << ", address "       << hex << address
        << ", data "       << hex << s_data
        << endl;
    }
#endif /* MASTER_LOG */
    if (cmd == tlm::TLM_READ_COMMAND)
      *data = s_data;

}

/*-----------------------------------------------------------------------------
 * Thread Name  : storeGPSData
 * Description  : Main function is to read data from GPS module and send
 *              : to memory1
 * Parameters   : void
 * Return Value : void
 *---------------------------------------------------------------------------*/
template<class T>
void master<T>::storeGPSData(void)
{
  char data = 0;
  uint32_t slaveAddr = SLAVE1_ADDR;
  while(1)
  {
    /* Read data from fifo */
    wait(muartRX01->data_written_event());
    data = muartRX01->read();
    if (data == '$')
      slaveAddress = slaveAddr;
    /* send Data to Memory1 */
    processData(slaveAddr, &data, tlm::TLM_WRITE_COMMAND);
    slaveAddr += 4;
  }
}

/*-----------------------------------------------------------------------------
 * Thread Name  : storeGSMData
 * Description  : Main function is to read data from GSM module and send
 *              : to memory2.
 * Parameters   : void
 * Return Value : void
 *---------------------------------------------------------------------------*/
template<class T>
void master<T>::storeGSMData(void)
{
  char data = 0;
  uint32_t slaveAddr2 = SLAVE2_ADDR;

  while(1)
  {
    wait(muartRX02->data_written_event());
    data = muartRX02->read();
    if (data == 'x')
    {
      cout << "Master - Read Req :  " << " @ " << sc_time_stamp()
        << ", data "       << hex << data
        << endl;
      cout << " " << endl;
      /* notify to read GPS data from memory */
      read_gps.notify(SC_ZERO_TIME);
    }
    else
    {
      /* Send GSM data to Memory2 */
      processData(slaveAddr2, &data, tlm::TLM_WRITE_COMMAND);
      slaveAddr2 = slaveAddr2 + 4;
    }
  }
}

/*-----------------------------------------------------------------------------
 * Thread Name  : getGPSData
 * Description  : Main function is to read GSM data from Memory1 and send data
 *              : to GSM module.
 * Parameters   : void
 * Return Value : void
 *---------------------------------------------------------------------------*/
template<class T>
void master<T>::getGPSData(void)
{
  char data = 0;
  int i = 0;
  unsigned int slaveAddr1 = SLAVE1;
  while(1)
  {
    /* wait till character 'x' is received from GSM module */
    wait(read_gps);
    slaveAddr1 = slaveAddress;
    while(1)
    {
      /* Read the data from the memory1 */
      processData(slaveAddr1, &data, tlm::TLM_READ_COMMAND);
      /* Read the data from the memory1 */
      muartTX02->write(data);
      slaveAddr1 += 4;
      wait(1, SC_NS);

      if(data == '*' || i > 0)
        i++;
      if (i > 2)
      {
        i = 0;
        break;
      }
    }
  }
}
