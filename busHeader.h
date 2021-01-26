#ifndef _BUS_HEADER_H_
#define _BUS_HEADER_H_

#include "systemc.h"

#define SSADDRESS              28
#define MSADDRESS              0xfffffff

#define ADDR_INCR              1


#define SLAVE1_ADDR            0x00000000
#define SLAVE2_ADDR            0x10000000

#define MEM_SIZE               1024
#define ADDR_INCR              1

#define RESET                 "\033[0m"
#define RED                   "\033[31m"      /* Red */
#define GREEN                 "\033[32m"      /* Green */
#define BOLDWHITE             "\033[1m\033[37m"


int errorCheck;

typedef enum
{
  SLAVE1,
  SLAVE2,
  NUM_SLAVE
}numSlave;

const sc_time cpuSleep(1, SC_NS);

#endif /* _BUS_HEADER_H_ */
