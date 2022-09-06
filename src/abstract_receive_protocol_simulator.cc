#include "abstract_receive_protocol_simulator.h"

#ifdef __ABS_RCV_PROT__

abstract_receive_protocol_simulator::abstract_receive_protocol_simulator (
  std::vector<stake_t> stakes)
  : corruptions_stake_protocol_simulator (stakes)
{
  this->requesters = std::vector<std::vector<party_t>> (this->nr_parties);
}

abstract_receive_protocol_simulator::~abstract_receive_protocol_simulator ()
{
}

counter_t
abstract_receive_protocol_simulator::handle_all_requests ()
{
  counter_t msgs = 0;

  for (party_t i = 0; i < this->nr_parties; i++)
  {
    if (has_received (i) && !is_corrupted (i))
    {
      msgs += this->handle_requests (i);
    } else
    {
      this->requesters[i] = std::vector<party_t> ();
    }
  }

  return msgs;
}

counter_t
abstract_receive_protocol_simulator::request (party_t requester)
{
  counter_t msgs = 0;

  if (is_corrupted (requester) || has_received (requester))
  {
    return msgs;
  }

  std::vector<party_t> targets = this->get_request_targets (requester);

  for (party_t target : targets)
  {
    msgs++;

    if (has_received (target) && !is_corrupted (target))
    {
      this->requesters[target].push_back (requester);
    }
  }
  return msgs;
}

counter_t
abstract_receive_protocol_simulator::make_requests ()
{
  counter_t msgs = 0;

  for (party_t i = 0; i < this->nr_parties; i++)
  {
    msgs += request (i);
  }

  return msgs;
}

std::pair<counter_t, bool>
abstract_receive_protocol_simulator::protocol_round ()
{
  __yal_log (__YAL_INFO,
             "<abstract_receive_protocol_simulator.cc:protocol_round>\n");

  counter_t msgs = make_requests ();

  msgs += handle_all_requests ();

  return std::make_pair (msgs, this->all_got_msg ());
}

#endif    // __ABS_RCV_PROT__
