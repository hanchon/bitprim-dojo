//
// Created by hanchon on 27/05/18.
//

#include <wallet/transaction_functions.hpp>
#include <bitcoin/bitcoin.hpp>
#include <utils/output.hpp>
#include <utils/input.hpp>
#include <utils/transaction.hpp>
#include <utils/script.hpp>
#include <utils/ec_private.hpp>
#include <utils/endorsement.hpp>

namespace bitprim {
static bool push_scripts(std::vector<libbitcoin::chain::output> &outputs,
                         const libbitcoin::explorer::config::output &output, uint8_t script_version) {
  static constexpr uint64_t no_amount = 0;

  // explicit script
  if (!output.is_stealth() && output.script().is_valid()) {
    outputs.push_back({output.amount(), output.script()});
    return true;
  }

  // If it's not explicit the script must be a form of pay to short hash.
  if (output.pay_to_hash() == libbitcoin::null_short_hash)
    return false;

  libbitcoin::machine::operation::list payment_ops;
  const auto hash = output.pay_to_hash();

  // This presumes stealth versions are the same as non-stealth.
  if (output.version() != script_version)
    payment_ops = libbitcoin::chain::script::to_pay_key_hash_pattern(hash);
  else if (output.version() == script_version)
    payment_ops = libbitcoin::chain::script::to_pay_script_hash_pattern(hash);
  else
    return false;

  // If stealth add null data stealth output immediately before payment.
  if (output.is_stealth())
    outputs.push_back({no_amount, output.script()});

  outputs.push_back({output.amount(), {payment_ops}});
  return true;
}

std::string bitprim_transaction::tx_encode(const std::vector<std::string> &output_to_spend,
                                           const std::vector<std::string> &destiny, const std::string &message) const {

  const uint32_t locktime = 0;
  const auto tx_version = 1;
  const auto script_version = 5;

  libbitcoin::chain::transaction tx;
  tx.set_version(tx_version);
  tx.set_locktime(locktime);

  for (const auto &input: output_to_spend) {
    tx.inputs().push_back(libbitcoin::explorer::config::input(input));
  }

  for (const auto &output: destiny) {
    if (!push_scripts(tx.outputs(), libbitcoin::explorer::config::output(output), script_version)) {
      std::cout << "Invalid output" << std::endl;
      return "Error";
    }
  }

  //// Experimental use of OP_RETURN
  if (message != "") {
    libbitcoin::data_chunk encoded_message;
    libbitcoin::decode_base16(encoded_message, message);
    //TODO: calculate the message's size and add that op_code before the message
    libbitcoin::machine::operation::list op_codes = {{libbitcoin::machine::opcode::return_}, {encoded_message}};
    tx.outputs().push_back({0, op_codes});
  }

  if (tx.is_locktime_conflict()) {
    std::cout << "Locktime conflict" << std::endl;
    return "Error";
  }

  libbitcoin::explorer::config::transaction res(tx);

  std::stringstream buffer;
  buffer << res;
  return buffer.str();
}

std::string bitprim_transaction::input_signature_old(const std::string &raw_private_key,
                                                     const std::string &raw_output_script,
                                                     const std::string &raw_tx, const int &index) const {

  // Bound parameters.
  const auto anyone_can_pay = false;

  // TODO:: the sign_type should be a parameter
  auto sign_type = 0x01; //all

  const libbitcoin::explorer::config::transaction temp_tx(raw_tx);
  const libbitcoin::chain::transaction tx(temp_tx);

  const libbitcoin::explorer::config::ec_private temp_priv(raw_private_key);
  const libbitcoin::ec_secret &private_key(temp_priv);

  const libbitcoin::explorer::config::script &temp_contract(raw_output_script);
  const libbitcoin::chain::script &contract(temp_contract);

  if (index >= tx.inputs().size()) {
    std::cout << "Input index out of range" << std::endl;
    return "Error";
  }

  uint8_t hash_type = (libbitcoin::machine::sighash_algorithm) sign_type;
  if (anyone_can_pay) {
    hash_type |= libbitcoin::machine::sighash_algorithm::anyone_can_pay;
  }

  libbitcoin::endorsement endorse;
  if (!libbitcoin::chain::script::create_endorsement(endorse, private_key, contract, tx, index,
                                                     hash_type)) {
    std::cout << "Input sign failed" << std::endl;
    return "Error";
  }
  return libbitcoin::encode_base16(endorse);
}

std::string bitprim_transaction::input_signature(const std::string &raw_private_key,
                                                 const std::string &raw_output_script,
                                                 const std::string &raw_tx,
                                                 const uint64_t &amount,
                                                 const int &index,
                                                 const bool bch) const {
  // Bound parameters.
  const auto anyone_can_pay = false;

  // TODO:: the sign_type should be a parameter
  auto sign_type = 0x01; //all
  if (bch) {
    sign_type |= 0x40;
  }

  const libbitcoin::explorer::config::transaction temp_tx(raw_tx);
  const libbitcoin::chain::transaction tx(temp_tx);

  const libbitcoin::explorer::config::ec_private temp_priv(raw_private_key);
  const libbitcoin::ec_secret &private_key(temp_priv);

  const libbitcoin::explorer::config::script &temp_contract(raw_output_script);
  const libbitcoin::chain::script &contract(temp_contract);

  if (index >= tx.inputs().size()) {
    std::cout << "Input index out of range" << std::endl;
    return "Error";
  }

  uint8_t hash_type = (libbitcoin::machine::sighash_algorithm) sign_type;
  if (anyone_can_pay) {
    hash_type |= libbitcoin::machine::sighash_algorithm::anyone_can_pay;
  }

  libbitcoin::endorsement endorse;
  // BCH enodrsement using script_version::zero (segwit sign algorithm)
  if (!libbitcoin::chain::script::create_endorsement(endorse,
                                                     private_key,
                                                     contract,
                                                     tx,
                                                     index,
                                                     hash_type,
                                                     libbitcoin::chain::script::script_version::zero,
                                                     amount)) {
    std::cout << "Input sign failed" << std::endl;
    return "Error";
  }

  return libbitcoin::encode_base16(endorse);
}

std::string bitprim_transaction::input_set(const std::string &signature,
                                           const std::string &public_key,
                                           const std::string &raw_tx) const {

  // Bound parameters.
  // TODO: index should be a parameter to set the value when inputs != 0
  const auto index = 0;
  const auto &tx_in = libbitcoin::explorer::config::transaction(raw_tx);
  const auto &script = libbitcoin::explorer::config::script("[" + signature + "] [" + public_key + "]");

  // Clone so we keep arguments const.
  auto tx_copy = libbitcoin::explorer::config::transaction(tx_in);
  auto &tx_out = tx_copy.data();

  if (index >= tx_out.inputs().size()) {
    std::cout << "Input index out of range" << std::endl;
    return "Error";
  }

  tx_out.inputs()[index].set_script(script);

  // TODO: when signing a segwit txn, the to_data function should use witness=true
  return libbitcoin::encode_base16(tx_out.to_data(true));

}

} //end namespace bitprim