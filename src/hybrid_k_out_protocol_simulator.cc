#include "hybrid_k_out_protocol_simulator.h"

#ifdef __HYBRID_K_OUT_PROT__

hybrid_k_out_protocol_simulator::hybrid_k_out_protocol_simulator (
  std::function<size_t (party_t)> get_nr_targets,
  std::function<size_t (party_t)> imitation_function,
  std::function<size_t (party_t)> get_nr_requests,
  std::function<size_t (party_t)> get_nr_requests_to_handle,
  std::vector<stake_t>            stakes)
  : corruptions_stake_protocol_simulator (stakes)
  , abstract_receive_protocol_simulator (stakes)
  , abstract_send_protocol_simulator (stakes)
  , receiver_protocol_simulator (
      get_nr_requests, get_nr_requests_to_handle, stakes)
  , k_out_protocol_simulator (get_nr_targets, imitation_function, stakes)
{
  this->get_nr_requests           = get_nr_requests;
  this->get_nr_requests_to_handle = get_nr_requests_to_handle;
}

hybrid_k_out_protocol_simulator::~hybrid_k_out_protocol_simulator ()
{
}

std::pair<counter_t, bool>
hybrid_k_out_protocol_simulator::protocol_round ()
{
  std::pair<counter_t, bool> recv_run_data =
    receiver_protocol_simulator::protocol_round ();

  std::pair<counter_t, bool> send_run_data =
    k_out_protocol_simulator::protocol_round ();

  counter_t msgs = std::get<0> (recv_run_data) + std::get<0> (send_run_data);
  bool      done = std::get<1> (recv_run_data) && std::get<1> (send_run_data);
  return std::make_pair (msgs, done);
}

#endif    // __HYBRID_K_OUT_PROT__
