#include "protocol_simulator.h"

#ifndef __CORRUPTIONS_STAKE_PROT_SIM__
#define __CORRUPTIONS_STAKE_PROT_SIM__

#include <random>
#include <algorithm>
#include <set>
#include <functional>
#include <ctime>
#include <cstdlib>

#include "yal_log.h"

/**
 * @enumate adversary_phase
 * @brief Enumerate for defining what phase the adversary is running.
 *
 * Enumerate to define the phases that an adversary can be run during the
 * execution of a protocol.
 */
enum adversary_phase
{
  begin,
  end
};

/**
 * @class corruptions_stake_protocol_simulator
 * @brief Abstract class for protocol simulators with stake and corruptions.
 *
 * Abstract base class for protocol simulators for message propagation with
 * stake and corruptions.
 */
class corruptions_stake_protocol_simulator : public protocol_simulator
{
    protected:
  // vector assigning to each party a stake
  std::vector<stake_t> stakes;

  // the total stake
  stake_t total_stake;

  // vector assigning to each party a corruption type
  std::vector<corruption_t> corrupted;

  /**
   * Returns the owner of a coin.
   * It is assumed that party 0 is the owner of the first stakes[0] coins, party
   * 1 is the owner of the next stakes[1] coins, etc.
   */
  party_t
  owner_of (stake_t c);

  virtual std::pair<counter_t, bool>
  round ();

  virtual std::pair<counter_t, bool>
  protocol_round () = 0;

    public:
  corruptions_stake_protocol_simulator (std::vector<stake_t> stakes);
  virtual ~corruptions_stake_protocol_simulator ();

  /**
   * Returns the vector of stakes of parties.
   */
  std::vector<stake_t>
  get_stakes ();

  /**
   * Returns the total stake of parties
   */
  stake_t
  get_total_stake ();

  std::vector<corruption_t>
  get_corrupted ();

  corruption_t
  is_corrupted (party_t i);

  void
  corrupt_parties (std::vector<corruption_t> corrupted);

  /**
   * Gets the latency in rounds of message propagation.
   * @return the last round at which a party received the message, or
   * ROUND_T_MAX if sender was not yet set.
   */
  virtual round_t
  get_latency ();

  /**
   * @return true if and only if every honest party got the message.
   */
  virtual bool
  all_got_msg ();

  virtual void
  adversary (party_t i, adversary_phase phase);
};

std::vector<corruption_t>
corrupt_parties_by_stake (
  std::vector<stake_t>                                             stakes,
  std::set<party_t>                                                honest,
  double                                                           max,
  std::function<void (std::vector<std::pair<party_t, stake_t>> &)> sort);

#endif    // __CORRUPTIONS_STAKE_PROT_SIM__
