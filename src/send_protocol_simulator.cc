#include "send_protocol_simulator.h"

#ifdef __SEND_PROT__

send_protocol_simulator::send_protocol_simulator (
  std::function<double (party_t, party_t)> send_prob,
  std::vector<stake_t>                     stakes)
  : corruptions_stake_protocol_simulator (stakes)
  , abstract_send_protocol_simulator (stakes)
  , send_prob (send_prob)
{
  this->re   = get_random_generator ();
  this->unif = std::uniform_real_distribution<double> (0.0, 1.0);
}

send_protocol_simulator::~send_protocol_simulator ()
{
  delete this->re;
}

std::vector<party_t>
send_protocol_simulator::get_targets_to_send (party_t sender)
{
  std::vector<party_t> targets;
  for (party_t i = 0; i < this->nr_parties; i++)
  {
    if (i != sender && this->send (sender, i))
    {
      targets.push_back (i);
    }
  }

  return targets;
}

bool
send_protocol_simulator::send (party_t sender, party_t receiver)
{
  double prob   = send_prob (sender, receiver);
  double sample = unif (*(this->re));
  return sample <= prob;
}

#endif    // __SEND_PROT__
