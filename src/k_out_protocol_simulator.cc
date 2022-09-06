#include "k_out_protocol_simulator.h"

#ifdef __K_OUT_PROT__

k_out_protocol_simulator::k_out_protocol_simulator (
  std::function<size_t (party_t)> get_nr_targets,
  std::function<size_t (party_t)> imitation_function,
  std::vector<stake_t>            stakes)
  : corruptions_stake_protocol_simulator (stakes)
  , abstract_send_protocol_simulator (stakes)
{
  this->re                      = get_random_generator ();
  this->get_nr_targets          = get_nr_targets;
  this->imitation_function      = imitation_function;
  this->histogram               = {};
  this->histogram_interval_size = 10;
  this->total_imitated_nodes    = 0;
  this->sample_domain           = {};


  // initializes the stake/party pair list
  for (size_t it = 0; it < nr_parties; it++)
  {
    size_t imitated_nodes = this->imitation_function (it);
    if (imitated_nodes == 0)
    {
      throw std::string (
        "<k_out_protocol_simulator.cc:k_out_protocol_simulator>: imitated "
        "nodes is 0");
    }
    sample_domain.push_back (std::make_pair (imitated_nodes, it));
    this->total_imitated_nodes += imitated_nodes;
  }
}

k_out_protocol_simulator::~k_out_protocol_simulator ()
{
  delete this->re;
}

bool
k_out_protocol_simulator::all_got_msg ()
{
  if (__yal_logging (__YAL_INFO))
  {
    __yal_log (__YAL_INFO, "Target histogram: [Party set]:\t Hit count\n");
    for (auto it = histogram.begin (); it != histogram.end (); it++)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
      size_t lb = it->first * histogram_interval_size;
      size_t ub = lb + histogram_interval_size - 1;
      __yal_log (__YAL_INFO, "[%lu; %lu]:\t %lu\n", lb, ub, it->second);
#pragma GCC diagnostic pop
    }
  }
  return corruptions_stake_protocol_simulator::all_got_msg ();
}

std::function<size_t (party_t)>
k_out_protocol_simulator::get_get_nr_targets ()
{
  return this->get_nr_targets;
}

std::function<size_t (party_t)>
k_out_protocol_simulator::get_imitation_function ()
{
  return this->imitation_function;
}

std::vector<party_t>
k_out_protocol_simulator::get_targets_to_send (party_t sender)
{
  party_t nr_targets = this->get_nr_targets (sender);

  nr_targets = nr_targets > this->nr_parties ? this->nr_parties : nr_targets;

  std::list<std::pair<size_t, party_t>> custom_sample_domain = sample_domain;

  // initializes the stake/party pair list, excluding the sender from the list
  for (auto it = custom_sample_domain.begin ();
       it != custom_sample_domain.end ();
       it++)
  {
    if (std::get<1> (*it) == sender)
    {
      it = custom_sample_domain.erase (it);
    }
  }

  std::vector<party_t> targets =
    sample (this->re,
            nr_targets,
            custom_sample_domain,
            this->nr_parties - 1,
            this->total_imitated_nodes - this->imitation_function (sender));

  if (__yal_logging (__YAL_INFO))
  {
    for (size_t i = 0; i < targets.size (); i++)
    {
      if (!histogram.count (targets[i] / histogram_interval_size))
      {
        histogram[targets[i] / histogram_interval_size] = 0;
      }
      histogram[targets[i] / histogram_interval_size]++;
    }

    // for debugging purposes
    __yal_log (__YAL_INFO, "Sender: %lu\n", sender);
    __yal_log (__YAL_INFO, "Receivers: [");
    for (size_t i = 0; i < targets.size (); i++)
    {
      __yal_log (__YAL_INFO, "  %lu", targets[i]);
    }
    __yal_log (__YAL_INFO, "]\n");
  }

  return targets;
}

#endif    // __K_OUT_PROT__
