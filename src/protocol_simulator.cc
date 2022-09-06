#include "protocol_simulator.h"

#ifdef __PROT_SIM__

protocol_simulator::protocol_simulator (party_t nr_parties)
  : nr_parties (nr_parties), curr_round (0)
{
  this->got_msg = std::vector<round_t> (nr_parties, ROUND_T_MAX);
}

protocol_simulator::~protocol_simulator ()
{
}

round_t
protocol_simulator::get_latency ()
{
  party_t start = 0;
  while (this->got_msg[start] == ROUND_T_MAX)
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
    if (this->got_msg[i] < ROUND_T_MAX && max < this->got_msg[i])
    {
      max = this->got_msg[i];
    }
  }

  return max;
}

party_t
protocol_simulator::get_nr_parties (round_t bound)
{
  party_t sum = 0;
  for (party_t i = 0; i < this->nr_parties; i++)
  {
    if (bound >= this->got_msg[i])
    {
      sum++;
    }
  }
  return sum;
}

party_t
protocol_simulator::get_nr_parties ()
{
  return this->nr_parties;
}

bool
protocol_simulator::all_got_msg ()
{
  for (party_t i = 0; i < this->nr_parties; i++)
  {
    if (this->got_msg[i] == ROUND_T_MAX)
    {
      return 0;
    }
  }
  return 1;
}

std::vector<round_t>
protocol_simulator::get_got_msg ()
{
  return this->got_msg;
}

bool
protocol_simulator::receive (party_t receiver)
{
  if (this->got_msg[receiver] != ROUND_T_MAX)
  {
    return 0;
  } else
  {
    this->got_msg[receiver] = this->curr_round + 1;
    return 1;
  }
}

bool
protocol_simulator::has_received (party_t receiver)
{
  return this->got_msg[receiver] != ROUND_T_MAX;
}

void
protocol_simulator::set_sender (party_t sender)
{
  this->got_msg[sender] = 0;
}

std::vector<counter_t>
protocol_simulator::get_nr_messages_per_round ()
{
  return this->nr_messages;
}

counter_t
protocol_simulator::get_nr_messages ()
{
  counter_t sum = 0;
  for (round_t i = 0; i < this->nr_messages.size (); i++)
  {
    sum += this->nr_messages[i];
  }
  return sum;
}

bool
protocol_simulator::execute_protocol (round_t bound)
{
  round_t curr = 0;

  std::pair<counter_t, bool> pair = std::make_pair (0, false);

  while (curr++ < bound && !std::get<1> (pair))
  {
    pair = this->round ();
    this->nr_messages.push_back (std::get<0> (pair));
  }

  return std::get<1> (pair);
}

#endif    // __PROT_SIM__
