//
// Created by hanchon on 27/05/18.
//

#include <iostream>

#include <wallet/wallet_functions.hpp>
#include <simple_web_client/requester.hpp>
#include <wallet/transaction_functions.hpp>

namespace bitprim {

void generate_addrs() {
  // Try the wallet functions

  bitprim_wallet wallet_functions;

  // The seed must be base 16 encoded and at least 32 char long.
  std::string priv_key_seed = "fffb587496cc54912bbcef874fa9a61a";

  // Creates a private key using the seed.
  auto priv_key = wallet_functions.generate_priv_key(priv_key_seed);
  std::cout << "Private key:                   "
            << libbitcoin::encode_base16(priv_key) << std::endl
            << std::endl;

  // Private key in WIF for mainnet
  bool mainnet = true;
  std::cout << "Mainnet wallet (WIF):          "
            << wallet_functions.priv_key_to_wif(priv_key, mainnet) << std::endl
            << std::endl;

  // Private key in WIF for testnet
  mainnet = false;
  std::cout << "Testnet wallet (WIF):          "
            << wallet_functions.priv_key_to_wif(priv_key, mainnet) << std::endl
            << std::endl;

  // Creates a public key using the private key.
  auto pub_key = wallet_functions.priv_key_to_public(priv_key);
  // pub_key.enconded() can be used to get the string
  std::cout << "Public key:                    " << pub_key << std::endl
            << std::endl;

  // Creates a public key compressed using the private key.
  auto pub_key_comp = wallet_functions.priv_key_to_public(priv_key, true);
  // pub_key_comp.enconded() can be used to get the string
  std::cout << "Public key Compressed:         " << pub_key_comp << std::endl
            << std::endl;

  // Creates a mainnet wallet using the public key.
  mainnet = true;
  auto new_wallet = wallet_functions.pub_key_to_addr(pub_key_comp, mainnet);
  std::cout << "Wallet Address (Mainnet):      " << new_wallet.encoded()
            << std::endl;
#ifdef BITPRIM_CURRENCY_BCH
  std::cout << "Wallet cashAddress (Mainnet):  " << new_wallet.encoded_cashaddr()
            << std::endl
            << std::endl;
#endif

#ifdef BITPRIM_CURRENCY_BCH
  // Creates a payment_address using a cashAddr string

  std::string mainnet_address = new_wallet.encoded_cashaddr();
  new_wallet = wallet_functions.cashAddr_to_payment_address(mainnet_address, mainnet);
  std::cout << "Wallet Address (Imported):     " << new_wallet.encoded()
            << std::endl;
  std::cout << "Wallet cashAddress (Imported): " << new_wallet.encoded_cashaddr()
            << std::endl
            << std::endl;
#endif

  // Creates a tesnet wallet using the public key.
  mainnet = false;
  new_wallet = wallet_functions.pub_key_to_addr(pub_key_comp, mainnet);
  std::cout << "Wallet Address (Testnet):      " << new_wallet.encoded()
            << std::endl;
#ifdef BITPRIM_CURRENCY_BCH
  std::cout << "Wallet cashAddress (Testnet):  " << new_wallet.encoded_cashaddr()
            << std::endl
            << std::endl;
#endif

#ifdef BITPRIM_CURRENCY_BCH
  // Creates a payment_address using a cashAddr string
  std::string testnet_address = new_wallet.encoded_cashaddr();
  new_wallet = wallet_functions.cashAddr_to_payment_address(testnet_address, mainnet);
  std::cout << "Wallet Address (Imported):     " << new_wallet.encoded()
            << std::endl;
  std::cout << "Wallet cashAddress (Imported): " << new_wallet.encoded_cashaddr()
            << std::endl
            << std::endl;
#endif

}

void send_curl() {
  http_requester requester("localhost:18336");

  std::string query = requester.create_query("getinfo", "\"\"");
  auto reply = requester.request(query);
  if (reply.first) {
    std::cout << reply.second << std::endl;
  }

  query = requester.create_query("getaddresstxids", "[{\"addresses\": [\"n3GNqMveyvaPvUbH469vDRadqpJMPc84JA\"]}]");
  auto reply2 = requester.request(query);
  if (reply2.first) {
    std::cout << reply2.second << std::endl;
  }

}

void create_bch_testnet_tx() {
  const std::string seed = "fffb587496cc54912bbcef874fa9a61a";

  bitprim::bitprim_transaction transaction;

  /******** Create transaction ********/
  std::vector<std::string> inputs;
  inputs.push_back("980de6ce12c29698d54323c6b0f358e1a9ae867598b840ee0094b9df22b07393:1");
  std::vector<std::string> outputs;
  outputs.push_back("mwx2YDHgpdfHUmCpFjEi9LarXf7EkQN6YG:199999000");

  const auto txn = transaction.tx_encode(inputs, outputs);

  /********* Sign the input *********/
  // Previous output values
  const std::string prev_output_script = "dup hash160 [b43ff4532569a00bcab4ce60f87cdeebf985b69a] equalverify checksig";
  const uint64_t prev_output_amount = 200000000;

  // Seed to private
  bitprim::bitprim_wallet wallet_functions;
  const auto priv_key = wallet_functions.generate_priv_key(seed);
  const auto priv_key_string = libbitcoin::encode_base16(priv_key);

  // Create signature
  const auto signature = transaction.input_signature(priv_key_string, prev_output_script, txn, prev_output_amount);

  /***** Add signature to the txn *****/
  // Public key (this is the same used to generate the prev_output_script
  const auto pub_key = wallet_functions.priv_key_to_public(priv_key, true);

  // Add the signature + pubkey to the txn
  const auto result_txn = transaction.input_set(signature, pub_key.encoded(), txn);

  std::cout << "BCH transaction: " << std::endl;
  std::cout << result_txn << std::endl;

  libbitcoin::data_chunk temp_txn;
  libbitcoin::decode_base16(temp_txn, result_txn);
  std::cout << "BCH txn hash: " << libbitcoin::encode_hash(libbitcoin::bitcoin_hash(temp_txn)) << std::endl;

}

void create_btc_testnet_tx() {
  // It's the same as the BCH txn but it can be signed with the old method (input_signature_old)
  // If the txn is going to be signed with the new method (input_signature), the bch parameter must be false

  const std::string seed = "fffb587496cc54912bbcef874fa9a61a";

  bitprim::bitprim_transaction transaction;

  /******** Create transaction ********/
  std::vector<std::string> inputs;
  inputs.push_back("47658befd5c1ba5b9e4f9db2d819a884033d2439b1a8e92e04a3ddfbb9c26bd4:0");
  std::vector<std::string> outputs;
  outputs.push_back("mwx2YDHgpdfHUmCpFjEi9LarXf7EkQN6YG:37200");

  const auto txn = transaction.tx_encode(inputs, outputs);

  /********* Sign the input *********/
  // Previous output values
  const std::string prev_output_script = "dup hash160 [b43ff4532569a00bcab4ce60f87cdeebf985b69a] equalverify checksig";
  // Amount is not required when signing with the old method
  //const uint64_t prev_output_amount = 38200;

  // Seed to private
  bitprim::bitprim_wallet wallet_functions;
  const auto priv_key = wallet_functions.generate_priv_key(seed);
  const auto priv_key_string = libbitcoin::encode_base16(priv_key);

  // Create signature
  const auto signature = transaction.input_signature_old(priv_key_string, prev_output_script, txn);

  /***** Add signature to the txn *****/
  // Public key (this is the same used to generate the prev_output_script
  const auto pub_key = wallet_functions.priv_key_to_public(priv_key, true);

  // Add the signature + pubkey to the txn
  const auto result_txn = transaction.input_set(signature, pub_key.encoded(), txn);

  std::cout << "BTC transaction: " << std::endl;
  std::cout << result_txn << std::endl;

  libbitcoin::data_chunk temp_txn;
  libbitcoin::decode_base16(temp_txn, result_txn);
  std::cout << "BTC txn hash: " << libbitcoin::encode_hash(libbitcoin::bitcoin_hash(temp_txn)) << std::endl;

}

} //end namespace bitprim

int main() {
  std::cout << "/*********** Wallet functions ***********/" << std::endl;
  bitprim::generate_addrs();
  std::cout << std::endl << "/*********** Sending curls ***********/" << std::endl;
  bitprim::send_curl();
  std::cout << std::endl << "/*********** Create a BCH txn ***********/" << std::endl;
  bitprim::create_bch_testnet_tx();
  std::cout << std::endl << "/*********** Create a BTC txn ***********/" << std::endl;
  bitprim::create_btc_testnet_tx();

  return 0;
}

