#include <vector>
#include <list>
#include <random>
#include <tuple>

#include "yal_log.h"

#ifndef __RANDOM_SAMPLER__
#define __RANDOM_SAMPLER__

std::vector<size_t>
sample (std::mt19937_64                     *re,
        size_t                               n,
        std::list<std::pair<size_t, size_t>> dist,
        size_t                               dist_size,
        size_t                               total_weight);


#endif    // __RANDOM_SAMPLER__
