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
  std::vector<stake_t> stakes;    //!< vector assigning to each party a stake
  stake_t              total_stake;    //!< the total stake
  std::vector<corruption_t>
    corrupted;    //!< vector assigning to each party a corruption type

  /**
   * Returns the owner of a coin.
   * It is assumed that party 0 is the owner of the first stakes[0] coins, party
   * 1 is the owner of the next stakes[1] coins, etc.
   * @param the coin.
   * @return the party who owns the coin.
   */
  party_t
  owner_of (stake_t c);

  /**
   * Runs a round of the protocol, which consists of:
   * 1) all corrupted parties are run;
   * 2) run protocol_round;
   * 3) all corrupted parties are again run.
   * @return a pair (nr_messages, finished), where nr_messages is the number of
   * messages sent during the round, and finished indicates whether the protocol
   * execution has finished.
   */
  virtual std::pair<counter_t, bool>
  round ();

  /**
   * Runs a round of the protocol (here, only the actual protocol being
   * simulated is run).
   * @return a pair (nr_messages, finished), where nr_messages is the number of
   * messages sent during the round, and finished indicates whether the protocol
   * execution has finished.
   */
  virtual std::pair<counter_t, bool>
  protocol_round () = 0;

    public:
  corruptions_stake_protocol_simulator (std::vector<stake_t> stakes);
  virtual ~corruptions_stake_protocol_simulator ();

  /**
   * Returns the vector of stakes of parties.
   * @return stake distribution.
   */
  std::vector<stake_t>
  get_stakes ();

  /**
   * Returns the total stake of parties
   * @return total stake.
   */
  stake_t
  get_total_stake ();

  /**
   * Returns the vector of corrupted parties.
   * @return vector if corrupted parties.
   */
  std::vector<corruption_t>
  get_corrupted ();

  /**
   * Returns a party's corruption flag (0 meaning the party is not corrupted).
   * @param the party.
   * @return the corruption flag of the input party.
   */
  corruption_t
  is_corrupted (party_t i);

  /**
   * Sets the corruption flag of each party.
   * @param the vector of corruption flags for each party.
   */
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

  /**
   * Runs the adversary. If the adversary is passive, this method has no effect.
   * @param the corrupted party being run.
   * @param the phase the corrupted party is running (so it can do different
   * things depending on which phase of the round the party is being run).
   */
  virtual void
  adversary (party_t i, adversary_phase phase);
};

/**
 * Outputs a vector of corruption flags for the given stake distribution, and
 * taking into account the parties that cannot be corrupted, the maximum
 * corruption threshold, and the sorting function which indicates the corruption
 * order.
 * @param the stake distribution.
 * @param the set of parties that cannot be corrupted.
 * @param the maximum corruption threshold.
 * @param the corruption strategy (which is defined by a sorting function, with
 * loss of generality)
 * @return a vector of corruption flags.
 */
std::vector<corruption_t>
corrupt_parties_by_stake (
  std::vector<stake_t>                                             stakes,
  std::set<party_t>                                                honest,
  double                                                           max,
  std::function<void (std::vector<std::pair<party_t, stake_t>> &)> sort);

#endif    // __CORRUPTIONS_STAKE_PROT_SIM__
