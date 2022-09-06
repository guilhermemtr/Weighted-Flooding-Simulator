#include "main_utils.h"

#ifdef __MAIN_UTILS__

void
init ()
{
  __yal_init_logger (); // initializes the logger
  __yal_set_lvl_err (); // sets the logging level
  init_random_utils (); // initialization of our randomness sources
}

std::vector<stake_t>
generate_stake_distribution (party_t           n,
                             double            param1,
                             long unsigned int param2,
                             unsigned int      flags)
{
  __yal_log (__YAL_INFO,
             "<main_utils.cc:generate_stake_distribution(%lu,%lf,%lu,%s)>\n",
             n,
             param1,
             param2,
             (std::bitset<TEST_TYPES> (flags)).to_string ().c_str ());

  std::vector<stake_t>         stakes (n);
  stake_distribution_generator sdgen;

  // Deterministic Distributions
  // test constant distribution
  if (flags & DET_CONST_DIST)
  {
    stakes = sdgen.constant_stake (n, 1);
  }

  // test few fat distribution, thin sender
  if (flags & DET_FF_THIN_DIST)
  {
    stakes = sdgen.few_fat_thin_stake (n, param1, param2);
  }

  // test few fat distribution, fat sender
  if (flags & DET_FF_FAT_DIST)
  {
    stakes = sdgen.few_fat_fat_stake (n, param1, param2);
  }

  // test exponential distribution, thinnest sender
  if (flags & DET_EXP_THIN_DIST)
  {
    stakes = sdgen.exponential_thinnest_stake (n, param1);
  }

  // test exponential distribution, median sender
  if (flags & DET_EXP_MEDN_DIST)
  {
    stakes = sdgen.exponential_median_stake (n, param1);
  }

  // test exponential distribution, thinnest sender
  if (flags & DET_EXP_FAT_DIST)
  {
    stakes = sdgen.exponential_fattest_stake (n, param1);
  }

  // Random Distributions
  // test random exponential distribution
  if (flags & RAND_EXP_RAND_SND_DIST)
  {
    stakes = sdgen.sample_exponential_stake (n, param1);
  }

  // test uniform distribution
  if (flags & RAND_UNIF_RAND_SND_DIST)
  {
    stakes = sdgen.sample_uniform_stake (n, 0, param1);
  }

  // test geometric distribution
  if (flags & RAND_GEOM_RAND_SND_DIST)
  {
    stakes = sdgen.sample_geometric_stake (n, param1);
  }

  /*if (flags & (0b1000 << 16))
  {
  // std::vector<stake_t> stakes =
  //   read_stake_distribution_file ("stake_distributions/0.stakes");
    sdgen.add_to_stake_distribution(stakes,1);

    // std::cout << "total stake: " << total_stake << std::endl;
    succeeded = succeeded && test_stake_distribution (stakes);
    }*/

  if (flags & (RAND_EXP_RAND_SND_DIST | RAND_UNIF_RAND_SND_DIST | RAND_GEOM_RAND_SND_DIST))
  {
    sdgen.add_to_stake_distribution (stakes, 1);
  }

  if (__yal_logging (__YAL_INFO))
  {
    __yal_log (__YAL_INFO,
               "<main_utils.cc:generate_stake_distribution return>:\n");
    stake_t total_stake = 0;
    for (size_t i = 0; i < n; i++)
    {
      total_stake += stakes[i];
    }

    for (size_t i = 0; i < n; i++)
    {
      __yal_log (
        __YAL_INFO,
        "\t party (stake; fraction of total stake): %lu\t (%lu; %lf)\n",
        i,
        stakes[i],
        ((double) stakes[i]) / ((double) total_stake));
    }
  }

  return stakes;
}

std::vector<corruption_t>
get_corrupted_parties (std::vector<stake_t> stakes,
                       std::set<party_t>    honest,
                       corruption_t         corruption_strategy,
                       double               corruption_threshold)
{
  __yal_log (__YAL_INFO,
             "<main_utils.cc:get_corrupted_parties(%lu,%lu,%lu,%lf)>\n",
             stakes.size (),
             honest.size (),
             corruption_strategy,
             corruption_threshold);

  std::vector<corruption_t> corruptions;

  switch (corruption_strategy)
  {
    case CORR_RAND:    // corrupt parties randomly
      corruptions = corrupt_parties_by_stake (
        stakes,
        honest,
        corruption_threshold,
        [] (std::vector<std::pair<party_t, stake_t>> &v) {
          std::mt19937_64 *g = get_random_generator ();
          std::shuffle (v.begin (), v.end (), *g);
        });
      break;

    case CORR_RICH_FIRST:    // corrupt rich first
      corruptions = corrupt_parties_by_stake (
        stakes,
        honest,
        corruption_threshold,
        [] (std::vector<std::pair<party_t, stake_t>> &v) {
          std::sort (v.begin (),
                     v.end (),
                     [] (std::pair<party_t, stake_t> ls,
                         std::pair<party_t, stake_t> rs) {
                       return std::get<1> (ls) > std::get<1> (rs);
                     });
        });
      break;

    case CORR_POOR_FIRST:    // corrupt poor first
      corruptions = corrupt_parties_by_stake (
        stakes,
        honest,
        corruption_threshold,
        [] (std::vector<std::pair<party_t, stake_t>> &v) {
          std::sort (v.begin (),
                     v.end (),
                     [] (std::pair<party_t, stake_t> ls,
                         std::pair<party_t, stake_t> rs) {
                       return std::get<1> (ls) < std::get<1> (rs);
                     });
        });
      break;

    case NO_CORR:
    default:
      corruptions = std::vector<corruption_t> (stakes.size (), 0);
      break;
  }

  if (__yal_logging (__YAL_DBG)) // if logging is on, print information about
                                 // the corruption set
  {
    size_t  corr_parties = 0;
    stake_t corr_stake   = 0;
    size_t  n            = stakes.size ();
    stake_t total_stake  = 0;
    for (size_t i = 0; i < n; i++)
    {
      total_stake += stakes[i];
    }

    for (size_t i = 0; i < n; i++)
    {
      corr_stake += corruptions[i] ? stakes[i] : 0;
      corr_parties += !!(corruptions[i]);
      __yal_log (__YAL_DBG, "<main_utils.cc:get_corrupted_parties return:>\n");

      __yal_log (
        __YAL_DBG,
        "party (stake; fraction of total stake): %lu\t (%lu; %lf): %d\n",
        i,
        stakes[i],
        ((double) stakes[i]) / ((double) total_stake),
        !!(corruptions[i]));
    }

    __yal_log (__YAL_DBG, "Corruption threshold: %lf\n", corruption_threshold);
    __yal_log (__YAL_DBG, "Number of parties: %lu\n", n);
    __yal_log (__YAL_DBG, "Total stake: %lu\n", total_stake);
    __yal_log (__YAL_DBG, "Dishonest stake: %lu\n", corr_stake);
  }

  return corruptions;
}

void
show_distribution_info (unsigned int flags, unsigned int to_show)
{
  if (to_show & SHOW_DISTRIBUTION)
  {
    switch (flags)
    {
      case 0b1:
        std::cout << "Deterministic Constant distribution";
        break;
      case 0b10:
        std::cout << "Deterministic Few Fat distribution, thin sender";
        break;
      case 0b100:
        std::cout << "Deterministic Few Fat distribution, fat sender";
        break;
      case 0b1000:
        std::cout << "Deterministic Exponential distribution, thinnest sender";
        break;
      case 0b10000:
        std::cout << "Deterministic Exponential distribution, median sender";
        break;
      case 0b100000:
        std::cout << "Deterministic Exponential distribution, fattest sender";
        break;
      case (0b1 << 16):
        std::cout << "Random Exponential distribution";
        break;
      case (0b10 << 16):
        std::cout << "Random Uniform distribution";
        break;
      case (0b100 << 16):
        std::cout << "Random Geometric distribution";
        break;
    }

    std::cout << std::endl;
  }
}

void
show_run_data (std::string               prot_name,
               double                    correct_runs,
               double                    runs,
               size_t                    factor,
               unsigned int              to_show,
               std::map<round_t, size_t> histogram)
{
  // To do: output the protocol name too; have to add this to the flags in the
  // read_arguments source/header files
  double success_rate = 100.00 * ((double) correct_runs) / ((double) runs);

  if (to_show & SHOW_FACTOR)
  {
    std::cout << "k = " << std::setw (3) << factor;
    if (to_show & SHOW_SUCCESS)
    {
      std::cout << ":";
    } else
    {
      std::cout << std::endl;
    }
  }

  if (to_show & SHOW_SUCCESS)
  {
    std::cout << std::setw (8) << success_rate << "%" << std::endl;
  }

  if (to_show & SHOW_HISTOGRAM)
  {
    std::cout << "Latency histogram:" << std::endl;
    for (auto it = histogram.begin (); it != histogram.end (); it++)
    {
      std::cout << it->first << "\t: " << it->second << std::endl;
    }
  }
}


template <typename T>
std::vector<T>
run_tests (party_t           n,
           unsigned int      flags,    // only has at most one bit set to 1.
           size_t            runs,
           corruption_t      corruption_strategy,
           double            corruption_threshold,
           size_t            factor,
           double            rich_poor_ratio,
           long unsigned int nr_rich,
           std::function<corruptions_stake_protocol_simulator *(
             std::vector<stake_t>, stake_t, size_t)> simulator_constructor,
           std::function<T (corruptions_stake_protocol_simulator *)> get_data)
{
  __yal_log (__YAL_INFO,
             "<main_utils.cc:run_tests(%lu,%s,%lu,%lu,%lf,%lu,%lf,%lu)>\n",
             n,
             (std::bitset<TEST_TYPES> (flags)).to_string ().c_str (),
             runs,
             corruption_strategy,
             corruption_threshold,
             factor,
             param1,
             param2);

  if (!flags)
  {
    std::string up = "<main_utils.cc:run_tests>: No flags were set";
    throw up;
  }

  std::vector<stake_t> stakes =
    generate_stake_distribution (n, rich_poor_ratio, nr_rich, flags);

  stake_t total_stake = 0;
  for (size_t i = 0; i < n; i++)
  {
    total_stake += stakes[i];
  }

  party_t sender = 0;

  std::vector<T> run_data (runs);

  // run each test of a series in parallel
#ifdef _OPENMP
#pragma omp parallel for
#endif    // _OPENMP

  for (counter_t c = 0; c < runs; c++)
  {
    corruptions_stake_protocol_simulator *sim =
      simulator_constructor (stakes, total_stake, factor);

    // Compute corrupted parties for each run, so that for non-deterministic
    // corruption strategies the set of corrupted parties varies from run to run
    sim->corrupt_parties (get_corrupted_parties (
      stakes, {sender}, corruption_strategy, corruption_threshold));

    sim->set_sender (sender);

    // this is the computationally heavy part
    sim->execute_protocol ();

#ifdef _OPENMP
#pragma omp critical
#endif    // _OPENMP
    {
      run_data[c] = get_data (sim);
    }

    delete sim;
  }

  return run_data;
}

template std::vector<std::pair<bool, round_t>>
run_tests (party_t           n,
           unsigned int      flags,    // only has at most one bit set to 1.
           size_t            runs,
           corruption_t      corruption_strategy,
           double            corruption_threshold,
           size_t            factor,
           double            rich_poor_ratio,
           long unsigned int nr_rich,
           std::function<corruptions_stake_protocol_simulator *(
             std::vector<stake_t>, stake_t, size_t)>  simulator_constructor,
           std::function<std::pair<bool, round_t> (
             corruptions_stake_protocol_simulator *)> get_data);



/*void
run_command_parameters (
  int                                       argc,
  char                                    **argv,
  std::function<corruptions_stake_protocol_simulator *(
    std::vector<stake_t>, stake_t, size_t)> simulator_constructor)
{
  __yal_init_logger ();
  __yal_set_lvl_info ();

  // to do: make the protocol name not hardcoded
  std::string prot_name = "";

  simulation_parameters_t params;
  try
  {
    params = read_parameters (argc, argv);
  } catch (std::string &e)
  {
    std::cout << "Something went wrong while reading command line parameters: "
              << e << std::endl;
    return;
  }

#ifdef __YAL_ON
  print_parameters (params);
#endif    // __YAL_ON

  init_random_utils ();

  unsigned int to_show = params.to_show;

  for (party_t n = params.min_nr_parties; n <= params.max_nr_parties; n *= 2)
  {
    if ((to_show & SHOW_NR_PARTIES))
    {
      std::cout << "Running for " << n << " parties" << std::endl;
    }

    for (unsigned int tc = 1; tc <= (1 << TEST_TYPES); tc *= 2)
    {
      if ((to_show & SHOW_TEST) && (tc & params.tests))
      {
        std::cout << "test " << std::bitset<TEST_TYPES> (tc & params.tests)
                  << std::endl;
      }

      show_distribution_info (tc & params.tests, to_show);

      for (size_t factor = params.min_factor; factor <= params.max_factor;
           factor++)
      {
        run_tests (NULL,
                   prot_name,
                   n,
                   tc & params.tests,
                   params.runs,
                   params.corruption_strategy,
                   params.corruption_threshold,
                   factor,
                   params.to_show,
                   params.ratio_rich_poor,
                   params.nr_rich_parties,
                   simulator_constructor);
      }
    }
  }
}
*/
#endif    // __MAIN_UTILS__
