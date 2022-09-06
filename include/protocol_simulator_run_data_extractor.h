#include <string>

#include <cstdlib>
#include <cstdio>

#ifndef __PROT_SIM_RUN_DATA_EXTR__
#define __PROT_SIM_RUN_DATA_EXTR__

#include "protocol_simulator.h"

class protocol_simulator;

class protocol_simulator_run_data_extractor
{
    protected:
  protocol_simulator *sim;

    public:
  protocol_simulator_run_data_extractor (protocol_simulator *sim);
  ~protocol_simulator_run_data_extractor ();

  protocol_simulator *
  get_protocol_simulator ();

  std::vector<party_t>
  get_senders ();
};

#endif    // __PROT_SIM_RUN_DATA_EXTR__
