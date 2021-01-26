#include "systemc.h"
#include "top.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

int exit_status = 0; //global

int sc_main (int argc, char* argv[])
{
  top<unsigned char> topx("TopBlock");
  
  cout << "Simulation started: Time resolution: "
       << sc_get_time_resolution() << endl;

  sc_start();

  if(!sc_end_of_simulation_invoked())
  {
    cout << "Simulation stopped: " << endl;
    sc_stop();
  }

  return exit_status;
}
