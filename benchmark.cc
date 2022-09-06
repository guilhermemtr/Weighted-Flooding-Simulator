#include "main_utils.h"
#include "random_utils.h"

#include <ctime>
#include <iostream>
#include <locale>

std::function<size_t (std::vector<stake_t>, size_t)>
  default_total_messages_calculator =
    [] (std::vector<stake_t> stakes, size_t factor) {
      stake_t total_stake = 0;
      for (size_t i = 0; i < stakes.size (); i++)
      {
        total_stake += stakes[i];
      }

      size_t msgs = 0;
      for (size_t i = 0; i < stakes.size (); i++)
      {
        size_t nr_targets =
          (size_t) ((ceil ((((double) (stakes[i] * stakes.size ()))
                            / ((double) total_stake))))
                    * (double) factor);
        if (nr_targets > stakes.size () - 1)
        {
          nr_targets = stakes.size () - 1;
        }
        msgs += nr_targets;
      }

      return msgs;
    };

// protocol_simulator constructors
std::function<corruptions_stake_protocol_simulator *(
  std::vector<stake_t>, stake_t, size_t)>
  default_simulator_constructor =
    [] (std::vector<stake_t> stakes, stake_t total_stake, size_t factor) {
      return new k_out_protocol_simulator (
        [&, stakes, total_stake, factor] (party_t i) {
          return (size_t) ((ceil ((((double) (stakes[i] * stakes.size ()))
                                   / ((double) total_stake))))
                           * (double) factor);
        },
        [&, stakes, total_stake, factor] (party_t i) {
          return (size_t) (ceil ((((double) (stakes[i] * stakes.size ()))
                                  / ((double) total_stake))));
        },
        stakes);
    };

std::function<size_t (std::vector<stake_t>, size_t)>
  stake_independent_total_messages_calculator =
    [] (std::vector<stake_t> stakes, size_t factor) {
      return stakes.size () * factor;
    };

std::function<corruptions_stake_protocol_simulator *(
  std::vector<stake_t>, stake_t, size_t)>
  stake_independent_simulator_constructor =
    [] (std::vector<stake_t> stakes, stake_t total_stake, size_t factor) {
      return new k_out_protocol_simulator (
        [&, stakes, total_stake, factor] (party_t i) { return factor; },
        [&, stakes, total_stake, factor] (party_t i) { return 1; },
        stakes);
    };

// configuration parameters
size_t                                    default_runs             = 1;
size_t                                    default_min_factor       = 1;
size_t                                    default_max_factor       = 50;
std::set<size_t>                          default_nr_parties_tests = {1024};
std::set<std::pair<corruption_t, double>> default_corruptions      = {
  std::make_pair (1, 0.5), std::make_pair (2, 0.5), std::make_pair (3, 0.5)};

std::set<std::tuple<unsigned int, double, unsigned int>> default_dists_unif = {
  std::make_tuple (0b000001, 0.0, 0)    // uniform dist
};

std::set<std::tuple<unsigned int, double, unsigned int>> default_dists_ff = {
  std::make_tuple (0b000010, 1000000.0, 10),    // few fat dist, thin snd
  std::make_tuple (0b000100, 1000000.0, 10)     // few fat dist, fat snd
};

std::set<std::tuple<unsigned int, double, unsigned int>> default_dists_exp = {
  std::make_tuple (0b001000, 1000000.0, 0),    // exp dist, thinnest snd
  std::make_tuple (0b010000, 1000000.0, 0),    // exp dist, median snd
  std::make_tuple (0b100000, 1000000.0, 0)     // exp dist, fattest snd
};

std::set<std::pair<std::set<std::tuple<unsigned int, double, unsigned int>>,
                   std::string>>
  default_dist_types = {std::make_pair (default_dists_unif, "unif"),
                        std::make_pair (default_dists_ff, "ff"),
                        std::make_pair (default_dists_exp, "exp")};

size_t
get_minimum_success_prob (std::vector<std::pair<bool, round_t>> test_data,
                          size_t                                runs)
{
  if (test_data.size () % runs)
  {
    std::string up =
      "<benchmark.cc:get_minimum_success_prob>:test_data is non-aligned with "
      "runs";
    throw up;
  }

  size_t curr_min      = runs;
  size_t nr_run_series = test_data.size () / runs;

  for (size_t j = 0; j < nr_run_series; j++)
  {
    size_t counter = 0;
    for (size_t i = 0; i < runs; i++)
    {
      counter += std::get<0> (test_data[j * runs + i]);
    }
    if (counter < curr_min)
    {
      curr_min = counter;
    }
  }

  return curr_min;
}

round_t
get_maximum_latency (std::vector<std::pair<bool, round_t>> test_data)
{
  size_t curr_max      = 0;
  size_t total_nr_runs = test_data.size ();

  for (size_t j = 0; j < total_nr_runs; j++)
  {
    if (std::get<0> (test_data[j]))
    {
      round_t latency = std::get<1> (test_data[j]);
      if (latency > curr_max)
      {
        curr_max = latency;
      }
    }
  }

  return curr_max;
}

FILE *
open_file_with_header (std::string fn, std::string header)
{
  std::string filename   = std::string ("results/") + fn;
  FILE       *run_output = fopen (filename.c_str (), "w");

  if (run_output == NULL)
  {
    throw std::string ("Could not open output csv file");
  }

  fprintf (run_output, header.c_str ());

  return run_output;
}

template <typename T>
void
plot (
  size_t                                               min_factor,
  size_t                                               max_factor,
  size_t                                               runs,
  std::set<size_t>                                     nr_parties_tests,
  std::set<std::pair<corruption_t, double>>            corruptions,
  std::set<std::pair<std::set<std::tuple<unsigned int, double, unsigned int>>,
                     std::string>>                     dist_types,
  std::function<corruptions_stake_protocol_simulator *(
    std::vector<stake_t>, stake_t, size_t)>            constructor,
  std::function<size_t (std::vector<stake_t>, size_t)> communication_calculator,
  std::string                                          protocol_name,
  std::function<T (corruptions_stake_protocol_simulator *)>    get_data,
  std::function<void (FILE *, std::vector<T>, size_t, size_t)> store_data,
  std::string                                                  prefix,
  std::string                                                  header)
{
  std::string fn_prefix = prefix + protocol_name;
  for (size_t nr_parties : nr_parties_tests)
  {
    std::string fn_prefix_nr = fn_prefix + "-" + std::to_string (nr_parties);
    for (std::pair<corruption_t, double> corr : corruptions)
    {
      std::string fn_prefix_nr_corr =
        fn_prefix_nr + "-" + std::to_string (std::get<0> (corr)) + "-"
        + std::to_string (std::get<1> (corr));
      for (std::pair<std::set<std::tuple<unsigned int, double, unsigned int>>,
                     std::string> dist_type : dist_types)
      {
        std::string fn_prefix_nr_corr_dist =
          fn_prefix_nr_corr + "-" + std::get<1> (dist_type) + ".csv";

        FILE *run_output =
          open_file_with_header (fn_prefix_nr_corr_dist, header);

        for (size_t factor = min_factor; factor <= max_factor; factor++)
        {
          std::vector<std::pair<bool, round_t>> full_data;

          for (std::tuple<unsigned int, double, unsigned int> dist :
               std::get<0> (dist_type))
          {
            std::vector<T> data = run_tests<T> (nr_parties,
                                                std::get<0> (dist),
                                                runs,
                                                std::get<0> (corr),
                                                std::get<1> (corr),
                                                factor,
                                                std::get<1> (dist),
                                                std::get<2> (dist),
                                                constructor,
                                                get_data);

            auto it = full_data.begin ();
            full_data.insert (it, data.begin (), data.end ());
          }

          std::tuple<unsigned int, double, unsigned int> val =
            std::make_tuple (0, 0, 0);

          for (std::tuple<unsigned int, double, unsigned int> dist :
               std::get<0> (dist_type))
          {
            val = dist;
          }

          std::vector<stake_t> dist =
            generate_stake_distribution (nr_parties,
                                         std::get<1> (val),
                                         std::get<2> (val),
                                         std::get<0> (val));

          size_t total_msgs = communication_calculator (dist, factor);
          double avg_comm   = ((double) total_msgs) / ((double) nr_parties);

          store_data (run_output, full_data, factor, avg_comm);
        }

        fclose (run_output);
      }
    }
  }
}

void
plot_graph_1_3 ()
{
  std::set<size_t> nr_parties_tests = {
    64, 128, 256, 512, 1024, 2048, 4096, 8192};
  std::set<std::pair<corruption_t, double>> corruptions = {
    std::make_pair (1, 0.5)};    // random corruptions

  std::set<std::tuple<unsigned int, double, unsigned int>> unif_dists = {
    std::make_tuple (0b000001, 0.0, 0)};
  std::set<std::pair<std::set<std::tuple<unsigned int, double, unsigned int>>,
                     std::string>>
    dist_types = {std::make_pair (unif_dists, "unif")};

  std::string fn_prefix = "plot-1-3";

  plot<std::pair<bool, round_t>> (
    default_min_factor,
    default_max_factor,
    default_runs,
    nr_parties_tests,
    corruptions,
    dist_types,
    default_simulator_constructor,
    default_total_messages_calculator,
    std::string ("kout"),
    [] (corruptions_stake_protocol_simulator *sim) {
      return std::make_pair<bool, round_t> (
        sim->protocol_simulator::all_got_msg (),
        sim->protocol_simulator::get_latency ());
    },
    [&] (FILE                                 *run_output,
         std::vector<std::pair<bool, round_t>> full_data,
         size_t                                factor,
         double                                msgs) {
      size_t min = get_minimum_success_prob (full_data, default_runs);
      double min_success_rate =
        100.00 * ((double) min) / ((double) default_runs);
      size_t max_latency = get_maximum_latency (full_data);

      fprintf (run_output,
               "%lu, %.6f, %lu, %.2lf\n",
               factor,
               min_success_rate,
               max_latency,
               msgs);
    },
    fn_prefix,
    std::string ("k, success, latency, expComm\n"));
}


void
plot_graph_2_4 ()
{
  std::string fn_prefix = "plot-2-4";
  plot<std::pair<bool, round_t>> (
    default_min_factor,
    default_max_factor,
    default_runs,
    default_nr_parties_tests,
    default_corruptions,
    default_dist_types,
    default_simulator_constructor,
    default_total_messages_calculator,
    std::string ("kout"),
    [] (corruptions_stake_protocol_simulator *sim) {
      return std::make_pair (sim->protocol_simulator::all_got_msg (),
                             sim->protocol_simulator::get_latency ());
    },
    [&] (FILE                                 *run_output,
         std::vector<std::pair<bool, round_t>> full_data,
         size_t                                factor,
         double                                msgs) {
      size_t min = get_minimum_success_prob (full_data, default_runs);
      double min_success_rate =
        100.00 * ((double) min) / ((double) default_runs);
      size_t max_latency = get_maximum_latency (full_data);

      fprintf (run_output,
               "%lu, %.6f, %lu, %.2lf\n",
               factor,
               min_success_rate,
               max_latency,
               msgs);
    },
    fn_prefix,
    std::string ("k, success, latency, expComm\n"));
}


void
plot_graph_5 ()
{
  size_t min_factor = 1;
  size_t max_factor = 200;

  std::set<std::pair<corruption_t, double>> corruptions = {
    std::make_pair (3, 0.5)};

  std::set<std::tuple<unsigned int, double, unsigned int>> one_dists_exp = {
    std::make_tuple (0b001000, 1.0, 0),    // exp dist, thinnest snd
    std::make_tuple (0b010000, 1.0, 0),    // exp dist, median snd
    std::make_tuple (0b100000, 1.0, 0)     // exp dist, fattest snd
  };

  std::set<std::tuple<unsigned int, double, unsigned int>> thousand_dists_exp =
    {
      std::make_tuple (0b001000, 1000.0, 0),    // exp dist, thinnest snd
      std::make_tuple (0b010000, 1000.0, 0),    // exp dist, median snd
      std::make_tuple (0b100000, 1000.0, 0)     // exp dist, fattest snd
    };

  std::set<std::tuple<unsigned int, double, unsigned int>> million_dists_exp = {
    std::make_tuple (0b001000, 1000000.0, 0),    // exp dist, thinnest snd
    std::make_tuple (0b010000, 1000000.0, 0),    // exp dist, median snd
    std::make_tuple (0b100000, 1000000.0, 0)     // exp dist, fattest snd
  };

  std::set<std::tuple<unsigned int, double, unsigned int>> billion_dists_exp = {
    std::make_tuple (0b001000, 1000000000.0, 0),    // exp dist, thinnest snd
    std::make_tuple (0b010000, 1000000000.0, 0),    // exp dist, median snd
    std::make_tuple (0b100000, 1000000000.0, 0)     // exp dist, fattest snd
  };

  std::set<std::pair<std::set<std::tuple<unsigned int, double, unsigned int>>,
                     std::string>>
    dist_types = {std::make_pair (one_dists_exp, "1"),
                  std::make_pair (thousand_dists_exp, "1k"),
                  std::make_pair (million_dists_exp, "1m"),
                  std::make_pair (billion_dists_exp, "1b")};

  std::string fn_prefix = "plot-5";

  plot<std::pair<bool, round_t>> (
    min_factor,
    max_factor,
    default_runs,
    default_nr_parties_tests,
    corruptions,
    dist_types,
    default_simulator_constructor,
    default_total_messages_calculator,
    std::string ("kout"),
    [] (corruptions_stake_protocol_simulator *sim) {
      return std::make_pair (sim->protocol_simulator::all_got_msg (),
                             sim->protocol_simulator::get_latency ());
    },
    [&] (FILE                                 *run_output,
         std::vector<std::pair<bool, round_t>> full_data,
         size_t                                factor,
         double                                msgs) {
      size_t min = get_minimum_success_prob (full_data, default_runs);
      double min_success_rate =
        100.00 * ((double) min) / ((double) default_runs);
      size_t max_latency = get_maximum_latency (full_data);

      fprintf (run_output,
               "%lu, %.6f, %lu, %.2lf\n",
               factor,
               min_success_rate,
               max_latency,
               msgs);
    },
    fn_prefix,
    std::string ("k, success, latency, expComm\n"));

  plot<std::pair<bool, round_t>> (
    min_factor,
    max_factor,
    default_runs,
    default_nr_parties_tests,
    corruptions,
    dist_types,
    stake_independent_simulator_constructor,
    stake_independent_total_messages_calculator,
    std::string ("kout-stake-independent"),
    [] (corruptions_stake_protocol_simulator *sim) {
      return std::make_pair<bool, round_t> (
        sim->protocol_simulator::all_got_msg (),
        sim->protocol_simulator::get_latency ());
    },
    [&] (FILE                                 *run_output,
         std::vector<std::pair<bool, round_t>> full_data,
         size_t                                factor,
         double                                msgs) {
      size_t min = get_minimum_success_prob (full_data, default_runs);
      double min_success_rate =
        100.00 * ((double) min) / ((double) default_runs);
      size_t max_latency = get_maximum_latency (full_data);

      fprintf (run_output,
               "%lu, %.6f, %lu, %.2lf\n",
               factor,
               min_success_rate,
               max_latency,
               msgs);
    },
    fn_prefix,
    std::string ("k, success, latency, expComm\n"));
}


void
plot_graph_6 ()
{
  size_t           min_factor       = default_min_factor;
  size_t           max_factor       = 200;
  std::set<size_t> nr_parties_tests = {
    64, 128, 256, 512, 1024, 2048, 4096, 8192};
  std::set<std::pair<corruption_t, double>> corruptions = {
    std::make_pair (0, 0)};

  std::set<std::tuple<unsigned int, double, unsigned int>> dists_unif = {
    std::make_tuple (0b000001, 0.0, 0)    // uniform dist
  };

  std::set<std::tuple<unsigned int, double, unsigned int>> dists_ff = {
    std::make_tuple (0b000100, 1000000.0, 10)    // few fat dist, fat snd
  };

  std::set<std::tuple<unsigned int, double, unsigned int>> dists_exp_1 = {
    std::make_tuple (0b100000, 1.0, 0)    // exp dist, fattest snd
  };

  std::set<std::tuple<unsigned int, double, unsigned int>> dists_exp_k = {
    std::make_tuple (0b100000, 1000.0, 0)    // exp dist, fattest snd
  };

  std::set<std::tuple<unsigned int, double, unsigned int>> dists_exp_m = {
    std::make_tuple (0b100000, 1000000.0, 0)    // exp dist, fattest snd
  };

  std::set<std::tuple<unsigned int, double, unsigned int>> dists_exp_b = {
    std::make_tuple (0b100000, 1000000000.0, 0)    // exp dist, fattest snd
  };

  std::set<std::pair<std::set<std::tuple<unsigned int, double, unsigned int>>,
                     std::string>>
    dist_types = {std::make_pair (dists_unif, "unif"),
                  std::make_pair (dists_ff, "ff"),
                  std::make_pair (dists_exp_1, "exp_1"),
                  std::make_pair (dists_exp_k, "exp_k"),
                  std::make_pair (dists_exp_m, "exp_m"),
                  std::make_pair (dists_exp_b, "exp_b")};

  std::string fn_prefix = "plot-6";

  /*  plot<std::pair<bool, round_t>> (
    min_factor,
    max_factor,
    1,
    default_nr_parties_tests,
    corruptions,
    dist_types,
    default_simulator_constructor,
    default_total_messages_calculator,
    std::string ("kout"),
    [] (corruptions_stake_protocol_simulator *sim) {
      return std::make_pair<bool, round_t> (
        sim->protocol_simulator::all_got_msg (),
        sim->protocol_simulator::get_latency ());
    },
    [&] (FILE                                 *run_output,
         std::vector<std::pair<bool, round_t>> full_data,
         size_t                                factor,
         double msgs) { fprintf (run_output, "%lu, %.2lf\n", factor, msgs); },
    fn_prefix,
    std::string ("k, expComm\n"));*/


  for (size_t nr_parties : nr_parties_tests)
  {
    std::string fn_prefix_nr = fn_prefix + "-" + std::to_string (nr_parties);
    for (std::pair<corruption_t, double> corr : corruptions)
    {
      std::string fn_prefix_nr_corr =
        fn_prefix_nr + "-" + std::to_string (std::get<0> (corr)) + "-"
        + std::to_string (std::get<1> (corr));
      for (std::pair<std::set<std::tuple<unsigned int, double, unsigned int>>,
                     std::string> dist_type : dist_types)
      {
        std::string fn_prefix_nr_corr_dist =
          fn_prefix_nr_corr + "-" + std::get<1> (dist_type) + ".csv";

        FILE *run_output = open_file_with_header (fn_prefix_nr_corr_dist,
                                                  std::string ("k, expComm\n"));

        for (std::tuple<unsigned int, double, unsigned int> dist :
             std::get<0> (dist_type))
        {
          for (size_t factor = min_factor; factor <= max_factor; factor++)
          {
            std::vector<stake_t> stakes =
              generate_stake_distribution (nr_parties,
                                           std::get<1> (dist),
                                           std::get<2> (dist),
                                           std::get<0> (dist));

            stake_t total_stake = 0;
            for (size_t i = 0; i < nr_parties; i++)
            {
              total_stake += stakes[i];
            }

            size_t total_msgs =
              default_total_messages_calculator (stakes, factor);

            double avg_comm = ((double) total_msgs) / ((double) nr_parties);

            fprintf (run_output, "%lu, %.2f\n", factor, avg_comm);
          }
        }

        fclose (run_output);
      }
    }
  }
}

int
main (int argc, char **argv)
{
  init ();

  plot_graph_1_3 ();
  plot_graph_2_4 ();
  plot_graph_5 ();
  plot_graph_6 ();

  return 0;
}
