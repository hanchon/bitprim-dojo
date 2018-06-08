//
// Created by hanchon on 27/05/18.
//

#ifndef BITPRIM_DOJO_TRANSACTION_FUNCTIONS_HPP
#define BITPRIM_DOJO_TRANSACTION_FUNCTIONS_HPP

#include <string>
#include <vector>
#include <bitcoin/bitcoin.hpp>

namespace bitprim {

class bitprim_transaction {
public:
  // Creates a transaction without the input script
  std::string tx_encode(const std::vector<std::string> &output_to_spend,
                        const std::vector<std::string> &destiny,
                        const std::string &message = "") const;

  // Creates a signature for an input using the new signature process
  std::string input_signature(const std::string &raw_private_key,
                              const std::string &raw_output_script,
                              const std::string &raw_tx,
                              const uint64_t &amount,
                              const int &index = 0,
                              const bool bch = true) const;

  // Creates a signature for an input using the legacy process
  std::string input_signature_old(const std::string &raw_private_key,
                                  const std::string &raw_output_script,
                                  const std::string &raw_tx, const int &index = 0) const;

  // Assign a signature to an input
  std::string input_set(const std::string &signature, const std::string &public_key, const std::string &rawtx) const;

};

} //end namespace bitprim

#endif //BITPRIM_DOJO_TRANSACTION_FUNCTIONS_HPP
