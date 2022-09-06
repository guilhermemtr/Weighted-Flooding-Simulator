#include "read_args.h"

#ifdef __SIM_READ_ARGS__

void
print_arguments ()
{
  std::string args =
    "<#runs> <tests> <corrupt_parties> <corruption_threshold in [0,1]> "
    "<#min_parties> <#max_parties> <ratio_rich_poor> <#rich_parties> <to_show "
    "(a to show everything)> <#min_factor> <#max_factor>";
  std::cout << "Expecting arguments: " << args << std::endl;

  std::cout << "Possible test flags:" << std::endl;
  std::cout << std::bitset<TEST_TYPES> (0b1) << "  -  "
            << "Deterministic Constant distribution" << std::endl;
  std::cout << std::bitset<TEST_TYPES> (0b10) << "  -  "
            << "Deterministic Few-Fat distribution, thin sender" << std::endl;
  std::cout << std::bitset<TEST_TYPES> (0b100) << "  -  "
            << "Deterministic Few-Fat distribution, fat sender" << std::endl;
  std::cout << std::bitset<TEST_TYPES> (0b1000) << "  -  "
            << "Deterministic Exponential distribution, thinnest sender"
            << std::endl;
  std::cout << std::bitset<TEST_TYPES> (0b10000) << "  -  "
            << "Deterministic Exponential distribution, median sender"
            << std::endl;
  std::cout << std::bitset<TEST_TYPES> (0b100000) << "  -  "
            << "Deterministic Exponential distribution, fattest sender"
            << std::endl;

  std::cout << std::bitset<TEST_TYPES> (0b1 << RAND_TEST_TYPE_OFFSET) << "  -  "
            << "Random Exponential distribution" << std::endl;
  std::cout << std::bitset<TEST_TYPES> (0b10 << RAND_TEST_TYPE_OFFSET)
            << "  -  "
            << "Random Uniform distribution" << std::endl;
  std::cout << std::bitset<TEST_TYPES> (0b100 << RAND_TEST_TYPE_OFFSET)
            << "  -  "
            << "Random Geometric distribution" << std::endl;
}


void
print_parameters (simulation_parameters_t params)
{
  std::cout << "Runs: " << params.runs << std::endl;

  std::cout << "Tests to be done: " << std::bitset<TEST_TYPES> (params.tests)
            << std::endl;

  std::cout << "Corruption strategy " << params.corruption_strategy
            << std::endl;
  std::cout << "Corruption threshold " << params.corruption_threshold
            << std::endl;

  std::cout << "Minimum number of parties: " << params.min_nr_parties
            << std::endl;
  std::cout << "Maximum number of parties: " << params.max_nr_parties
            << std::endl;

  std::cout << "Ratio between rich and poor: " << params.ratio_rich_poor
            << std::endl;
  std::cout << "Number of rich parties: " << params.nr_rich_parties
            << std::endl;

  std::cout << "Output flags: " << std::bitset<NR_OUTPUTS_SHOW> (params.to_show)
            << std::endl;


  std::cout << "Minimum factor (k): " << params.min_factor << std::endl;
  std::cout << "Maximum factor (k): " << params.max_factor << std::endl;
}

simulation_parameters_t
read_parameters (int argc, char **argv)
{
  __yal_log (__YAL_INFO, "<read_args.cc:read_parameters (%d, [", argc);

  for (int i = 1; i < argc - 1; i++)
  {
    __yal_log (__YAL_INFO, "\"%s\", ", argv[i]);
  }
  if (argc > 1)
  {
    __yal_log (__YAL_INFO, "\"%s\" ", argv[argc - 1]);
  }
  __yal_log (__YAL_INFO, "]\n");


  if (argc < NR_ARGS)
  {
    std::cout << "Unexpected number of arguments" << std::endl;
    print_arguments ();
    throw std::string ("<read_args.cc:read_parameters>: wrong arguments");
  }

  std::string str_runs (argv[1]);

  std::string str_tests (argv[2]);

  std::string str_corruption_strategy (argv[3]);
  std::string str_corruption_threshold (argv[4]);

  std::string str_min_parties (argv[5]);
  std::string str_max_parties (argv[6]);

  std::string str_ratio_rich_poor (argv[7]);
  std::string str_nr_rich_parties (argv[8]);

  std::string str_to_show (argv[9]);

  std::string str_min_factor (argv[10]);
  std::string str_max_factor (argv[11]);


  size_t runs = 0;

  try
  {
    runs = std::stoi (str_runs.c_str ());
  } catch (std::invalid_argument const &ex)
  {
    std::cout << "std::invalid_argument::what(): " << ex.what () << std::endl;
  } catch (std::out_of_range const &ex)
  {
    std::cout << "std::out_of_range::what(): " << ex.what () << std::endl;
  }


  // all tests set to run
  unsigned int tests = 0;

  try
  {
    tests = std::stoi (str_tests.c_str (), nullptr, 2);
  } catch (std::invalid_argument const &ex)
  {
    std::cout << "std::invalid_argument::what(): " << ex.what () << std::endl;
  } catch (std::out_of_range const &ex)
  {
    std::cout << "std::out_of_range::what(): " << ex.what () << std::endl;
  }


  // decide whether to corrupt parties or not
  unsigned int corruption_strategy = 0;

  try
  {
    corruption_strategy = std::stoi (str_corruption_strategy.c_str ());
  } catch (std::invalid_argument const &ex)
  {
    std::cout << "std::invalid_argument::what(): " << ex.what () << std::endl;
  } catch (std::out_of_range const &ex)
  {
    std::cout << "std::out_of_range::what(): " << ex.what () << std::endl;
  }

  // set the corruption threshold
  double corruption_threshold = 0;

  try
  {
    corruption_threshold = std::stod (str_corruption_threshold.c_str ());
  } catch (std::invalid_argument const &ex)
  {
    std::cout << "std::invalid_argument::what(): " << ex.what () << std::endl;
  } catch (std::out_of_range const &ex)
  {
    std::cout << "std::out_of_range::what(): " << ex.what () << std::endl;
  }



  party_t min_nr_parties = 0;
  party_t max_nr_parties = 0;

  try
  {
    min_nr_parties = std::stol (str_min_parties.c_str ());
    max_nr_parties = std::stol (str_max_parties.c_str ());
  } catch (std::invalid_argument const &ex)
  {
    std::cout << "std::invalid_argument::what(): " << ex.what () << std::endl;
  } catch (std::out_of_range const &ex)
  {
    std::cout << "std::out_of_range::what(): " << ex.what () << std::endl;
  }

  // set the ratio_rich_poor
  double ratio_rich_poor = 0.0;

  try
  {
    ratio_rich_poor = std::stod (str_ratio_rich_poor.c_str ());
  } catch (std::invalid_argument const &ex)
  {
    std::cout << "std::invalid_argument::what(): " << ex.what () << std::endl;
  } catch (std::out_of_range const &ex)
  {
    std::cout << "std::out_of_range::what(): " << ex.what () << std::endl;
  }

  party_t nr_rich_parties = 0;

  try
  {
    nr_rich_parties = std::stol (str_nr_rich_parties.c_str ());
  } catch (std::invalid_argument const &ex)
  {
    std::cout << "std::invalid_argument::what(): " << ex.what () << std::endl;
  } catch (std::out_of_range const &ex)
  {
    std::cout << "std::out_of_range::what(): " << ex.what () << std::endl;
  }

  // all tests set to run
  unsigned int to_show = 0;

  try
  {
    if (!str_to_show.compare (std::string ("a")))
    {
      to_show = -1 & (0b11111);    // sets all main show flags to 1, except for
                                   // the histogram flag.
    } else if (!str_to_show.compare (std::string ("h")))
    {
      to_show = -1 & (0b111111);    // sets all main show flags to 1.
    } else if (!str_to_show.compare (std::string ("f")))
    {
      to_show = -1;    // sets all show flags to 1 (verbose).
    } else
    {
      to_show = std::stoi (str_to_show.c_str (), nullptr, 2);
    }
  } catch (std::invalid_argument const &ex)
  {
    std::cout << "std::invalid_argument::what(): " << ex.what () << std::endl;
  } catch (std::out_of_range const &ex)
  {
    std::cout << "std::out_of_range::what(): " << ex.what () << std::endl;
  }



  size_t min_factor = 0;
  size_t max_factor = 0;

  try
  {
    min_factor = std::stol (str_min_factor.c_str ());
    max_factor = std::stol (str_max_factor.c_str ());
  } catch (std::invalid_argument const &ex)
  {
    std::cout << "std::invalid_argument::what(): " << ex.what () << '\n';
  } catch (std::out_of_range const &ex)
  {
    std::cout << "std::out_of_range::what(): " << ex.what () << '\n';
  }

  return {.runs                 = runs,
          .tests                = tests,
          .corruption_strategy  = corruption_strategy,
          .corruption_threshold = corruption_threshold,
          .min_nr_parties       = min_nr_parties,
          .max_nr_parties       = max_nr_parties,
          .ratio_rich_poor      = ratio_rich_poor,
          .nr_rich_parties      = nr_rich_parties,
          .to_show              = to_show,
          .min_factor           = min_factor,
          .max_factor           = max_factor};
}



#endif    // __SIM_READ_ARGS__
