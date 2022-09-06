#include <utility>
#include <tuple>

#include "types.h"
#include "yal_log.h"

#ifndef __PROT_SIM__
#define __PROT_SIM__

/**
 * @class protocol_simulator
 * @brief Abstract class for protocol simulators.
 *
 * Abstract base class for protocol simulators for message propagation.
 */
class protocol_simulator
{
    protected:
  /**
   * Number of parties participating in the protocol execution.
   */
  party_t nr_parties;

  /**
   * Current round of the protocol execution.
   * The beginning of a protocol's execution is at round 0.
   */
  round_t curr_round;

  /**
   * Vector that has the number of messages sent per round.
   */
  std::vector<counter_t> nr_messages;

  /**
   * For each party i:
   *   got_msg[i] = ROUND_T_MAX,  if i did not yet get the message;
   *   got_msg[i] = r,            if i got the message at round r.
   */
  std::vector<round_t> got_msg;

  /**
   * Abstract method that runs a round of the protocol.
   * @return a pair (nr_messages, finished), where nr_messages is the number of
   * messages sent during the round, and finished indicates whether the protocol
   * execution has finished.
   */
  virtual std::pair<counter_t, bool>
  round () = 0;

    public:
  /**
   * Given a number of parties, constructs a new protocol_simulator object.
   */
  protocol_simulator (party_t nr_parties);

  /**
   * Destructor of protocol_simulator objects.
   */
  virtual ~protocol_simulator ();

  /**
   * Gets the latency in rounds of message propagation.
   * @return the last round at which a party received the message, or
   * ROUND_T_MAX if sender was not yet set.
   */
  virtual round_t
  get_latency ();

  /**
   * Gets the number of parties who got the message up to round r.
   * @param the round bound.
   * @return the number of parties who got the message up to round r.
   */
  virtual party_t
  get_nr_parties (round_t r);

  /**
   * Gets the number of parties.
   * @return the number of parties participating in the protocol execution.
   */
  party_t
  get_nr_parties ();

  /**
   * @return true if and only if every party got the message.
   */
  virtual bool
  all_got_msg ();

  /**
   * @return vector of rounds at which each party got the message.
   */
  std::vector<round_t>
  get_got_msg ();

  /**
   * Makes a party receive a message if it hasn't received it yet. Returns
   * whether the receiver received the message for the first time or not.
   *
   * @param party receiving a message.
   * @return true if and only if the party is receiving the message for the
   * first time.
   */
  bool
  receive (party_t receiver);

  /**
   * Checks if a party got a message.
   *
   * @param party to check.
   * @return true if and only if the party has already received the message.
   */
  bool
  has_received (party_t receiver);

  /**
   * Sets a sender.
   * @param sender of the message.
   */
  virtual void
  set_sender (party_t sender);

  /**
   * Gets the number of messages sent per round.
   * @return vector of number of messages sent per round.
   */
  virtual std::vector<counter_t>
  get_nr_messages_per_round ();

  /**
   * Gets the total number of messages sent.
   * @return the total number of messages sent during the execution of the
   * protocol.
   */
  virtual counter_t
  get_nr_messages ();

  /**
   * Runs the protocol for up to bound rounds.
   * @param bound on the number of rounds to be run.
   * @return whether the protocol's execution finished.
   */
  virtual bool
  execute_protocol (round_t bound = ROUND_T_MAX);
};

#endif    // __PROT_SIM__
