#include <iostream>
#include <queue>
#include <unordered_set>
#include <list>
#include <functional>
#include <unordered_map>
#include <map>

#include "abstract_send_protocol_simulator.h"
#include "random_sampler.h"
#include "random_utils.h"
#include "yal_log.h"

#ifndef __K_OUT_PROT__
#define __K_OUT_PROT__

class k_out_protocol_simulator : virtual public abstract_send_protocol_simulator
{
    protected:
  // variables used to get randomness
  std::mt19937_64                      *re; // the randomness generator
  std::function<size_t (party_t)>       get_nr_targets; // the function that
                                                        // outputs the number of
                                                        // targets of a given party
  std::function<size_t (party_t)>       imitation_function; // the imitation function
  std::list<std::pair<size_t, party_t>> sample_domain; // the sampling domain

  size_t total_imitated_nodes;

  size_t histogram_interval_size; // interval size of the histogram (how many
                                  // parties should be put together in the histogram).
  std::map<party_t, size_t>
    histogram;    // histogram of which parties were targeted

  /**
   * Decides whether the sender should send a message to the receiver.
   * @param sender.
   * @return the parties to send to.
   */
  std::vector<party_t>
  get_targets_to_send (party_t sender);

    public:
  k_out_protocol_simulator (std::function<size_t (party_t)> get_nr_targets,
                            std::function<size_t (party_t)> imitation_function,
                            std::vector<stake_t>            stakes);
  virtual ~k_out_protocol_simulator ();

  /**
   * Returns the function that specifies how many targets a party should send a
   * message to.
   * @return the function specifying how many targets a party should send a
   * message to.
   */
  std::function<size_t (party_t)> 
  get_get_nr_targets ();
  
  /**
   * Returns the imitation function.
   * @return the imitation function.
   */
  std::function<size_t (party_t)> 
  get_imitation_function ();

  /**
   * Returns whether all parties already got the message.
   * @return whether all parties already got the message.
   */
  bool
  all_got_msg ();
};

#endif    // __K_OUT_PROT__
