#include "random_sampler.h"

#ifdef __RANDOM_SAMPLER__

std::vector<size_t>
sample_yours (std::mt19937_64                     *re,
              size_t                               n,
              std::list<std::pair<size_t, size_t>> dist,
              size_t                               dist_size,
              size_t                               total_weight)
{
  std::vector<size_t> picks (0);

  std::vector<size_t> sample_domain (dist_size);
  for (std::pair<size_t, size_t> p : dist)
  {
    sample_domain[std::get<1> (p)] = std::get<0> (p);
  }

  while (picks.size () < n)
  {
    std::discrete_distribution<size_t> dd (sample_domain.begin (),
                                           sample_domain.end ());
    size_t                             to = dd (*re);
    sample_domain[to]                     = 0;
    picks.push_back (to);
  }

  return picks;
}


std::vector<size_t>
sample (std::mt19937_64                     *re,
        size_t                               n,
        std::list<std::pair<size_t, size_t>> dist,
        size_t                               dist_size,
        size_t                               total_weight)
{
  std::vector<size_t> picks (n);
  if (n >= dist_size)
  {
    size_t i = 0;
    for (auto it = dist.begin (); it != dist.end (); it++, i++)
    {
      picks[i] = std::get<1> (*it);
    }
    for (; i < n; i++)
    {
      picks[i] = std::get<1> (*(dist.begin ()));
    }
    return picks;
  }

  size_t curr_total_weight = total_weight;
  size_t pick_counter      = 0;

  while (pick_counter < n)
  {
    if (curr_total_weight == 0)
    {
      std::string up =
        "<random_sampler.cc:sample()>: Overflow due to miscalculation of "
        "curr_total_weight";
      throw up;
    }

    size_t curr_total_weight_bound = curr_total_weight - 1;

    std::uniform_int_distribution<size_t> d (0, curr_total_weight_bound);

    size_t node = d (*re);

    std::list<std::pair<size_t, size_t>>::iterator it  = dist.begin ();
    std::list<std::pair<size_t, size_t>>::iterator end = dist.end ();

    size_t passed_weights = 0;
    while (it != end)
    {
      size_t curr_weight = std::get<0> (*it);
      size_t curr_party  = std::get<1> (*it);
      passed_weights += curr_weight;
      if (node < passed_weights)    //  || node == 0
      {
        picks[pick_counter++] = curr_party;
        if (curr_total_weight <= curr_total_weight - curr_weight)
        {
          std::string up =
            "<random_sampler.cc:sample()>: Overflow due to miscalculation";
          throw up;
        }
        curr_total_weight -= curr_weight;
        dist.erase (it);
        it = end;
      } else
      {
        it++;
      }
    }
  }

  return picks;
}

#endif    // __RANDOM_SAMPLER__
