#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "types.h"

#ifndef __IO_STAKE_DISTRIBUTION__
#define __IO_STAKE_DISTRIBUTION__

/**
 * Reads a stake distribution from the file with name given as input to the
 * call.
 * @param the file path
 * @return a vector of stakes representing the stake distribution
 */
std::vector<stake_t>
read_stake_distribution_file (std::string fn);

/**
 * Writes a given stake distribution to a file.
 * @param the stake distribution.
 * @param the output file path.
 */
void
write_stake_distribution_file (std::vector<stake_t> dist, std::string fn);

/**
 * Writes a given stake distribution to a given output stream.
 * @param the stake distribution.
 * @param the output stream.
 */
void
output_stake_distribution (std::vector<stake_t> dist, FILE *f);


#endif    // __IO_STAKE_DISTRIBUTION__
