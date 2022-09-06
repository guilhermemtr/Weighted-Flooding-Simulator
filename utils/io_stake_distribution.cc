#include "io_stake_distribution.h"

std::vector<stake_t>
read_stake_distribution_file (std::string fn)
{
  std::vector<stake_t> stakes;

  FILE *file_reader;
  if ((file_reader = fopen (fn.c_str (), "r")) == NULL)
  {
    return stakes;
  }

  stake_t s;
  while (fscanf (file_reader, "%ld", &s) == 1)
  {
    stakes.push_back (s);
  }
  fclose (file_reader);

  return stakes;
}


void
write_stake_distribution_file (std::vector<stake_t> stakes, std::string fn)
{
  FILE *file_writer;
  if ((file_writer = fopen (fn.c_str (), "w")) == NULL)
  {
    std::string up = "write_stake_distribution_file: Failed to open file";
    throw up;
  }

  output_stake_distribution (stakes, file_writer);

  fclose (file_writer);
}


void
output_stake_distribution (std::vector<stake_t> stakes, FILE *f)
{
  for (party_t i = 0; i < stakes.size (); i++)
  {
    fprintf (f, "%ld\n", stakes[i]);
  }
}
