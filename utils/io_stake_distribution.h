#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "types.h"

#ifndef __IO_STAKE_DISTRIBUTION__
#define __IO_STAKE_DISTRIBUTION__


std::vector<stake_t>
read_stake_distribution_file (std::string fn);

void
write_stake_distribution_file (std::vector<stake_t> dist, std::string fn);

void
output_stake_distribution (std::vector<stake_t> dist, FILE *f);


#endif    // __IO_STAKE_DISTRIBUTION__
