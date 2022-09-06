#include "protocol_simulator_run_data_extractor.h"

#ifndef __CORRUPT_STAKE_PROT_SIM_RUN_DATA_EXTR__
#define __CORRUPT_STAKE_PROT_SIM_RUN_DATA_EXTR__

#include "corruptions_stake_protocol_simulator.h"

class corruptions_stake_protocol_simulator;

class corruptions_stake_protocol_simulator_run_data_extractor
  : public protocol_simulator_run_data_extractor
{
    public:
  corruptions_stake_protocol_simulator_run_data_extractor (
    corruptions_stake_protocol_simulator *sim);
  ~corruptions_stake_protocol_simulator_run_data_extractor ();


  virtual stake_t
  reached_stake (round_t i);

  stake_t
  corrupted_stake ();

  // statistics of interest (for later):
  // total stake
  // total honest stake
  // total dishonest stake
  // stake distribution
  // honest stake distribution
  // dishonest stake distribution
  // how much honest stake reached until each round
  // how much honest stake reached until each round as a fraction
};

#endif    // __CORRUPT_STAKE_PROT_SIM_RUN_DATA_EXTR__
