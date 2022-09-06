#include "send_protocol_simulator.h"
#include "receiver_protocol_simulator.h"

#ifndef __HYBRID_PROT__
#define __HYBRID_PROT__

class hybrid_protocol_simulator : public receiver_protocol_simulator,
                                  public send_protocol_simulator
{
    protected:
  virtual std::pair<counter_t, bool>
  protocol_round () override;

    public:
  hybrid_protocol_simulator (
    std::function<double (party_t, party_t)> send_prob,
    std::function<size_t (party_t)>          get_nr_requests,
    std::function<size_t (party_t)>          get_nr_requests_to_handle,

    std::vector<stake_t> stakes);
  ~hybrid_protocol_simulator ();
};

#endif    // __HYBRID_PROT__
