#include <string>

#include <cstdlib>
#include <cstdio>

#ifndef __PROT_SIM_STATS__
#define __PROT_SIM_STATS__

#include "protocol_simulator_run_data_extractor.h"

class protocol_simulator_run_data_extractor;

class protocol_simulator_statistics
{
    protected:
  size_t              nr_runs;
  protocol_simulator *sim;

    public:
  protocol_simulator_run_data_extractor (protocol_simulator *sim,
                                         bool                succ       = true,
                                         bool                diam       = true,
                                         bool                nr_parties = true,
                                         bool nr_messages               = false,
                                         bool nr_messages_per_round     = false,
                                         bool got_msg_dist              = false,
                                         bool sender                    = true);
  ~protocol_simulator_run_data_extractor ();

  virtual void
  set_statistics (bool succ                  = true,
                  bool diam                  = true,
                  bool nr_parties            = true,
                  bool nr_messages           = false,
                  bool nr_messages_per_round = false,
                  bool got_msg_dist          = false,
                  bool sender                = true);

  virtual void
  print_statistics ();

  virtual void
  store_statistics (std::string fn);
};

#endif    // __PROT_SIM_RUN_DATA_EXTR__
