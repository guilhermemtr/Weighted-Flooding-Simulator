#include <queue>
#include <functional>

#include "corruptions_stake_protocol_simulator.h"
#include "random_utils.h"

#ifndef __ABS_SEND_PROT__
#define __ABS_SEND_PROT__

class abstract_send_protocol_simulator
  : virtual public corruptions_stake_protocol_simulator
{
    protected:
  // queue of parties to send the message to in the next round
  std::queue<party_t>                          to_send;
  std::function<bool (bool, party_t, party_t)> forward;

  virtual std::vector<party_t>
  get_targets_to_send (party_t sender) = 0;

  /**
   * Runs a round of the protocol.
   */
  virtual std::pair<counter_t, bool>
  protocol_round () override;

    public:
  abstract_send_protocol_simulator (
    std::vector<stake_t>                         stakes,
    std::function<bool (bool, party_t, party_t)> forward =
      [] (bool first_time, party_t sender, party_t receiver) {
        return first_time;
      });
  ~abstract_send_protocol_simulator ();

  /**
   * Sets the sender of the message.
   */
  virtual void
  set_sender (party_t sender) override;
};

#endif    // __ABS_SEND_PROT__
