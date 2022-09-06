#include "corruptions_stake_protocol_simulator_run_data_extractor.h"

#ifdef __CORRUPT_STAKE_PROT_SIM_RUN_DATA_EXTR__

corruptions_stake_protocol_simulator_run_data_extractor::
  corruptions_stake_protocol_simulator_run_data_extractor (
    corruptions_stake_protocol_simulator *sim)
  : protocol_simulator_run_data_extractor (sim)
{
}

corruptions_stake_protocol_simulator_run_data_extractor::
  ~corruptions_stake_protocol_simulator_run_data_extractor ()
{
}

stake_t
corruptions_stake_protocol_simulator_run_data_extractor::reached_stake (
  round_t r)
{
  corruptions_stake_protocol_simulator *sim =
    dynamic_cast<corruptions_stake_protocol_simulator *> (this->sim);
  stake_t sum = 0;
  for (party_t i = 0; i < sim->nr_parties; i++)
  {
    if (sim->got_msg[i] <= r && !sim->corrupted[i])
    {
      sum += sim->stakes[i];
    }
  }
  return sum;
}


stake_t
corruptions_stake_protocol_simulator_run_data_extractor::corrupted_stake ()
{
  corruptions_stake_protocol_simulator *sim =
    dynamic_cast<corruptions_stake_protocol_simulator *> (this->sim);
  stake_t sum = 0;
  for (party_t i = 0; i < sim->nr_parties; i++)
  {
    if (sim->corrupted[i])
    {
      sum += sim->stakes[i];
    }
  }
  return sum;
}

#endif    // __CORRUPT_STAKE_PROT_SIM_RUN_DATA_EXTR__
