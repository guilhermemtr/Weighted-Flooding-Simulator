#include "abstract_send_protocol_simulator.h"

#ifdef __ABS_SEND_PROT__

abstract_send_protocol_simulator::abstract_send_protocol_simulator (
  std::vector<stake_t>                         stakes,
  std::function<bool (bool, party_t, party_t)> forward)
  : corruptions_stake_protocol_simulator (stakes), forward (forward)
{
  this->to_send = std::queue<party_t> ();
}

abstract_send_protocol_simulator::~abstract_send_protocol_simulator ()
{
}

std::pair<counter_t, bool>
abstract_send_protocol_simulator::protocol_round ()
{
  __yal_log (__YAL_INFO,
             "<abstract_send_protocol_simulator.cc:protocol_round>\n");

  if (to_send.empty ())
  {
    return std::make_pair (0, true);
  }

  counter_t           msgs_sent  = 0;
  std::queue<party_t> next_round = std::queue<party_t> ();

  while (!to_send.empty ())
  {
    party_t curr = to_send.front ();
    to_send.pop ();

    if (this->is_corrupted (curr))
    {
      continue;
    }

    std::vector<party_t> targets = get_targets_to_send (curr);

    for (size_t i = 0; i < targets.size (); i++)
    {
      msgs_sent++;
      if (this->forward (receive (targets[i]), i, targets[i]))
      {
        next_round.push (targets[i]);
      }
    }
  }

  this->to_send = next_round;
  this->curr_round++;

  return std::make_pair (msgs_sent, this->to_send.empty ());
}

void
abstract_send_protocol_simulator::set_sender (party_t sender)
{
  protocol_simulator::set_sender (sender);
  this->to_send.push (sender);
}

#endif    // __ABS_SEND_PROT__
