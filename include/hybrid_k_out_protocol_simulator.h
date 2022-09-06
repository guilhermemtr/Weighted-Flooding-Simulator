#include <iostream>
#include <queue>
#include <unordered_set>
#include <list>
#include <functional>
#include <unordered_map>

#include "random_utils.h"
#include "receiver_protocol_simulator.h"
#include "k_out_protocol_simulator.h"

#ifndef __HYBRID_K_OUT_PROT__
#define __HYBRID_K_OUT_PROT__

class hybrid_k_out_protocol_simulator : public receiver_protocol_simulator,
                                        public k_out_protocol_simulator
{
    protected:
  // variables used to get randomness
  std::function<size_t (party_t)> get_nr_requests;
  std::function<size_t (party_t)> get_nr_requests_to_handle;

  /**
   * Runs a round of the protocol.
   */
  virtual std::pair<counter_t, bool>
  protocol_round () override;

    public:
  hybrid_k_out_protocol_simulator (
    std::function<size_t (party_t)> get_nr_targets,
    std::function<size_t (party_t)> imitation_function,
    std::function<size_t (party_t)> get_nr_requests,
    std::function<size_t (party_t)> get_nr_requests_to_handle,
    std::vector<stake_t>            stakes);
  virtual ~hybrid_k_out_protocol_simulator ();
};

#endif    // __HYBRID_K_OUT_PROT__
