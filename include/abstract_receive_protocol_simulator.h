#include <vector>
#include <functional>

#include "corruptions_stake_protocol_simulator.h"
#include "random_utils.h"

#ifndef __ABS_RCV_PROT__
#define __ABS_RCV_PROT__

class abstract_receive_protocol_simulator
  : virtual public corruptions_stake_protocol_simulator
{
    protected:
  std::vector<std::vector<party_t>> requesters;

  counter_t
  handle_all_requests ();

  virtual counter_t
  handle_requests (party_t i) = 0;

  counter_t
  request (party_t requester);

  counter_t
  make_requests ();

  virtual std::vector<party_t>
  get_request_targets (party_t requester) = 0;

  /**
   * Runs a round of the protocol.
   */
  virtual std::pair<counter_t, bool>
  protocol_round () override;

    public:
  abstract_receive_protocol_simulator (std::vector<stake_t> stakes);
  ~abstract_receive_protocol_simulator ();
};

#endif    // __ABS_RCV_PROT__
