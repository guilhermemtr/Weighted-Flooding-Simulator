#include "receiver_protocol_simulator.h"

#ifdef __RCV_PROT__

receiver_protocol_simulator::receiver_protocol_simulator (
  std::function<size_t (party_t)> get_nr_requests,
  std::function<size_t (party_t)> get_nr_requests_to_handle,
  std::vector<stake_t>            stakes)
  : corruptions_stake_protocol_simulator (stakes)
  , abstract_receive_protocol_simulator (stakes)
{
  this->re = get_random_generator ();
  this->unif_coin =
    std::uniform_int_distribution<party_t> (0, this->total_stake - 1);
}

receiver_protocol_simulator::~receiver_protocol_simulator ()
{
  delete this->re;
}

std::vector<party_t>
receiver_protocol_simulator::get_request_targets (party_t requester)
{
  std::vector<party_t> targets;
  size_t               requests_to_make = this->get_nr_requests (requester);

  requests_to_make = !requests_to_make ? 1 : requests_to_make;

  for (size_t req = 0; req < requests_to_make; req++)
  {
    stake_t target_coin = unif_coin (*(this->re));
    party_t target      = owner_of (target_coin);
    targets.push_back (target);
  }

  return targets;
}

counter_t
receiver_protocol_simulator::handle_requests (party_t i)
{
  counter_t msgs               = 0;
  size_t    requests_to_handle = this->get_nr_requests_to_handle (i);
  size_t    req                = 0;

  requests_to_handle = !requests_to_handle ? 1 : requests_to_handle;

  while (req < requests_to_handle && !this->requesters[i].empty ())
  {
    party_t requester = this->requesters[i].back ();
    this->requesters[i].pop_back ();
    this->receive (requester);
    msgs++;
    req++;
  }

  return msgs;
}

#endif    // __RCV_PROT__
