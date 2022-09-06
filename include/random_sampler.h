#include <vector>
#include <list>
#include <random>
#include <tuple>

#include "yal_log.h"

#ifndef __RANDOM_SAMPLER__
#define __RANDOM_SAMPLER__
/**
 * Samples n values from a given distribution without repetition.
 * @param a pointer to the random engine to be used.
 * @param the number of values to be sampled.
 * @param the distribution.
 * @param the size of the distribution.
 * @param the total weight of the distribution.
 * @return a vector containing n values sampled according to the distribution,
 * and without repetition.
 */
std::vector<size_t>
sample (std::mt19937_64                     *re,
        size_t                               n,
        std::list<std::pair<size_t, size_t>> dist,
        size_t                               dist_size,
        size_t                               total_weight);


#endif    // __RANDOM_SAMPLER__
