#include <vector>
#include <cstddef>
#include <climits>

#ifndef __PROT_SIM_TYPES__
#define __PROT_SIM_TYPES__

// Defines some basic types

typedef std::size_t       party_t;
typedef std::size_t       counter_t;
typedef unsigned long int round_t;
typedef unsigned long int corruption_t;
typedef unsigned long int stake_t;

#define PARTY_T_MAX SIZE_MAX
#define COUNTER_T_MAX SIZE_MAX
#define ROUND_T_MAX ULONG_MAX
#define CORRUPTION_T_MAX ULONG_MAX
#define STAKE_T_MAX ULONG_MAX
// USHRT_MAX



#endif    // __PROT_SIM_TYPES__
