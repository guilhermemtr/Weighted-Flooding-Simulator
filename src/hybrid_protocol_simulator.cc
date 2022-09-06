#include "hybrid_protocol_simulator.h"

#ifdef __HYBRID_PROT__

hybrid_protocol_simulator::hybrid_protocol_simulator (
  std::function<double (party_t, party_t)> send_prob,
  std::function<size_t (party_t)>          get_nr_requests,
  std::function<size_t (party_t)>          get_nr_requests_to_handle,
  std::vector<stake_t>                     stakes)
  : corruptions_stake_protocol_simulator (stakes)
  , abstract_receive_protocol_simulator (stakes)
  , abstract_send_protocol_simulator (stakes)
  , receiver_protocol_simulator (
      get_nr_requests, get_nr_requests_to_handle, stakes)
  , send_protocol_simulator (send_prob, stakes)
{
}

hybrid_protocol_simulator::~hybrid_protocol_simulator ()
{
}

std::pair<counter_t, bool>
hybrid_protocol_simulator::protocol_round ()
{
  std::pair<counter_t, bool> recv_run_data =
    receiver_protocol_simulator::protocol_round ();

  std::pair<counter_t, bool> send_run_data =
    send_protocol_simulator::protocol_round ();

  counter_t msgs = std::get<0> (recv_run_data) + std::get<0> (send_run_data);
  bool      done = std::get<1> (recv_run_data) && std::get<1> (send_run_data);
  return std::make_pair (msgs, done);
}

#endif    // __HYBRID_PROT__
