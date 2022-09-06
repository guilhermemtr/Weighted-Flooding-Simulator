#include "random_sampler.h"

#ifdef __RANDOM_SAMPLER__

std::vector<size_t>
sample (std::mt19937_64                     *re,
        size_t                               n,
        std::list<std::pair<size_t, size_t>> dist,
        size_t                               dist_size,
        size_t                               total_weight)
{
  std::vector<size_t> picks (n);
  
  // if the number of values to be picked is larger than the number of possible
  // values of the distribution, output a vector containing all the different
  // values of the distribution.
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

  // picks more values while not enough have been picked.
  while (pick_counter < n)
  {
    // check for possible overflows
    if (curr_total_weight == 0)
    {
      std::string up =
        "<random_sampler.cc:sample()>: Overflow due to miscalculation of "
        "curr_total_weight";
      throw up;
    }

    // sample a value uniformly at random from 0 up to the current total weight
    size_t curr_total_weight_bound = curr_total_weight - 1;
    std::uniform_int_distribution<size_t> d (0, curr_total_weight_bound);
    size_t node = d (*re);

    // find the value corresponding to the sampled coin, add this value to
    // vector picks, and then remove the value from the distribution (so we can
    // repeat the procedure all over again for the next value to be picked).
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
        // adds the sampled value to the set of picked values
        picks[pick_counter++] = curr_party;

        // check for possible overflows
        if (curr_total_weight <= curr_total_weight - curr_weight)
        {
          std::string up =
            "<random_sampler.cc:sample()>: Overflow due to miscalculation";
          throw up;
        }
        
        // update the total weight
        curr_total_weight -= curr_weight;

        //erase the sampled value from the distribution
        dist.erase (it);

        // exit the while loop
        it = end;
      } else
      {
        it++;
      }
    }
  }

  return picks;
}



//std::vector<size_t>
//sample (std::mt19937_64                     *re,
//        size_t                               n,
//        std::list<std::pair<size_t, size_t>> dist,
//        size_t                               dist_size,
//        size_t                               total_weight)
//{
//  std::vector<size_t> picks (0);
//
//  std::vector<size_t> sample_domain (dist_size);
//  for (std::pair<size_t, size_t> p : dist)
//  {
//    sample_domain[std::get<1> (p)] = std::get<0> (p);
//  }
//
//  while (picks.size () < n)
//  {
//    std::discrete_distribution<size_t> dd (sample_domain.begin (),
//                                           sample_domain.end ());
//    size_t                             to = dd (*re);
//    sample_domain[to]                     = 0;
//    picks.push_back (to);
//  }
//
//  return picks;
//}


#endif    // __RANDOM_SAMPLER__
