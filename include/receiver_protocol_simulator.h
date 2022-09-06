#include "abstract_receive_protocol_simulator.h"
#include "random_utils.h"

#ifndef __RCV_PROT__
#define __RCV_PROT__

class receiver_protocol_simulator
  : virtual public abstract_receive_protocol_simulator
{
    protected:
  std::mt19937_64                       *re;
  std::uniform_int_distribution<party_t> unif_coin;
  std::function<size_t (party_t)>        get_nr_requests;
  std::function<size_t (party_t)>        get_nr_requests_to_handle;

  party_t
  request (party_t requester);

  counter_t
  handle_requests (party_t i);

  std::vector<party_t>
  get_request_targets (party_t requester);

    public:
  receiver_protocol_simulator (
    std::function<size_t (party_t)> get_nr_requests,
    std::function<size_t (party_t)> get_nr_requests_to_handle,
    std::vector<stake_t>            stakes);
  ~receiver_protocol_simulator ();
};

#endif    // __RCV_PROT__
