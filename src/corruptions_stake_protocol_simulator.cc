#include "corruptions_stake_protocol_simulator.h"

#ifdef __CORRUPTIONS_STAKE_PROT_SIM__

corruptions_stake_protocol_simulator::corruptions_stake_protocol_simulator (
  std::vector<stake_t> stakes)
  : protocol_simulator (stakes.size ()), stakes (stakes)
{
  this->corrupted   = std::vector<corruption_t> (this->nr_parties, 0);
  this->total_stake = 0;
  for (party_t i = 0; i < this->nr_parties; i++)
  {
    this->total_stake += stakes[i];
  }
}

corruptions_stake_protocol_simulator::~corruptions_stake_protocol_simulator ()
{
}

std::vector<stake_t>
corruptions_stake_protocol_simulator::get_stakes ()
{
  return this->stakes;
}

stake_t
corruptions_stake_protocol_simulator::get_total_stake ()
{
  return this->total_stake;
}

party_t
corruptions_stake_protocol_simulator::owner_of (stake_t c)
{
  if (c >= this->total_stake)
  {
    return STAKE_T_MAX;
  }

  stake_t sum = 0;
  for (party_t i = 0; i < this->nr_parties; i++)
  {
    sum += stakes[i];
    if (c < sum)
    {
      return i;
    }
  }
  return this->nr_parties - 1;
}

std::vector<corruption_t>
corruptions_stake_protocol_simulator::get_corrupted ()
{
  return this->corrupted;
}

corruption_t
corruptions_stake_protocol_simulator::is_corrupted (party_t i)
{
  return this->corrupted[i];
}

void
corruptions_stake_protocol_simulator::corrupt_parties (
  std::vector<corruption_t> corrupted)
{
  this->corrupted = corrupted;
}

void
corruptions_stake_protocol_simulator::adversary (party_t         i,
                                                 adversary_phase phase)
{
  // This corresponds to a fail-stop party
}

round_t
corruptions_stake_protocol_simulator::get_latency ()
{
  party_t start = 0;
  while (this->got_msg[start] == ROUND_T_MAX || this->corrupted[start])
  {
    start++;
    if (start >= this->nr_parties)
    {
      return ROUND_T_MAX;
    }
  }

  round_t max = this->got_msg[start];

  for (party_t i = start; i < this->nr_parties; i++)
  {
    if (this->got_msg[i] < ROUND_T_MAX && max < this->got_msg[i]
        && !this->corrupted[i])
    {
      max = this->got_msg[i];
    }
  }

  return max;
}

bool
corruptions_stake_protocol_simulator::all_got_msg ()
{
  bool some_honest_got_msg = 0;
  for (party_t i = 0; i < this->nr_parties; i++)
  {
    if (this->got_msg[i] < ROUND_T_MAX && !this->corrupted[i])
    {
      some_honest_got_msg = 1;
      break;
    }
  }

  if (!some_honest_got_msg)
  {
    return 1;
  }

  bool ret_value = 1;

  for (party_t i = 0; i < this->nr_parties; i++)
  {
    if (this->got_msg[i] == ROUND_T_MAX && !this->corrupted[i])
    {
      if (__yal_logging (__YAL_INFO))
      {
        __yal_log (__YAL_INFO,
                   "<corruptions_stake_protocol_simulator.cc::all_got_msg>: "
                   "Party %lu did not receive message\n",
                   i);
        ret_value = 0;
      } else
      {
        return 0;
      }
    }
  }
  return ret_value;
}

std::pair<counter_t, bool>
corruptions_stake_protocol_simulator::round ()
{
  for (party_t i = 0; i < this->nr_parties; i++)
  {
    if (this->is_corrupted (i))
    {
      this->adversary (i, adversary_phase::begin);
    }
  }

  std::pair<counter_t, bool> ret_val = this->protocol_round ();

  for (party_t i = 0; i < this->nr_parties; i++)
  {
    if (this->is_corrupted (i))
    {
      this->adversary (i, adversary_phase::begin);
    }
  }

  return ret_val;
}

std::vector<corruption_t>
corrupt_parties_by_stake (
  std::vector<stake_t>                                             stakes,
  std::set<party_t>                                                honest,
  double                                                           max,
  std::function<void (std::vector<std::pair<party_t, stake_t>> &)> sort)
{
  party_t                                  nr_parties = stakes.size ();
  std::vector<corruption_t>                corrupted_parties (nr_parties, 0);
  stake_t                                  corrupted_stake = 0;
  size_t                                   nr_honest       = honest.size ();
  std::vector<std::pair<party_t, stake_t>> parties (nr_parties - nr_honest);

  // creates vector for sorting
  for (party_t i = 0, h = 0; i < nr_parties; i++)
  {
    if (honest.count (i))
    {
      h++;
      continue;
    }
    parties[i - h] = std::make_pair (i, stakes[i]);
  }

  stake_t total_stake = 0;
  for (size_t i = 0; i < nr_parties; i++)
  {
    total_stake += stakes[i];
  }

  sort (parties);


  // corrupts the parties
  for (party_t i = 0; i < nr_parties - nr_honest; i++)
  {
    if (corrupted_stake + std::get<1> (parties[i])
        <= (stake_t) (max * ((double) total_stake)))
    {
      corrupted_parties[std::get<0> (parties[i])] = 1;
      corrupted_stake += std::get<1> (parties[i]);
    }
  }

  if (__yal_logging (__YAL_DBG))
  {
    for (party_t i = 0; i < nr_parties; i++)
    {
      __yal_log (__YAL_DBG,
                 "Party %lu: \t %lu\t %u\n",
                 i,
                 stakes[i],
                 !!(corrupted_parties[i]));
    }
  }

  return corrupted_parties;
}

#endif    // __CORRUPTIONS_STAKE_PROT_SIM__
