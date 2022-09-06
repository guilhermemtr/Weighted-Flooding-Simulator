#include "protocol_simulator_run_data_extractor.h"

#ifdef __PROT_SIM_RUN_DATA_EXTR__

protocol_simulator_run_data_extractor::protocol_simulator_run_data_extractor (
  protocol_simulator *sim)
{
  this->sim = sim;
}

protocol_simulator_run_data_extractor::~protocol_simulator_run_data_extractor ()
{
}

protocol_simulator *
protocol_simulator_run_data_extractor::get_protocol_simulator ()
{
  return sim;
}

std::vector<party_t>
protocol_simulator_run_data_extractor::get_senders ()
{
  std::vector<party_t> senders (0);
  for (party_t i = 0; i < this->sim->nr_parties; i++)
  {
    if (this->sim->got_msg[i] == 0)
    {
      senders.push_back (i);
    }
  }
  return senders;
}

#endif    // __PROT_SIM_RUN_DATA_EXTR__
