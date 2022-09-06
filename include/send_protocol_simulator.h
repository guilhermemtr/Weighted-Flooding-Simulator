#include <queue>
#include <functional>

#include "abstract_send_protocol_simulator.h"
#include "random_utils.h"

#ifndef __SEND_PROT__
#define __SEND_PROT__

class send_protocol_simulator : virtual public abstract_send_protocol_simulator
{
    protected:
  // variables used to get randomness
  std::mt19937_64                       *re;
  std::uniform_real_distribution<double> unif;

  // function describing probability distribution for sending message
  std::function<double (party_t, party_t)> send_prob;

  /**
   * Decides whether the sender should send a message to the receiver.
   */
  bool
  send (party_t sender, party_t receiver);

  virtual std::vector<party_t>
  get_targets_to_send (party_t sender);

    public:
  send_protocol_simulator (std::function<double (party_t, party_t)> send_prob,
                           std::vector<stake_t>                     stakes);
  ~send_protocol_simulator ();
};

#endif    // __SEND_PROT__
