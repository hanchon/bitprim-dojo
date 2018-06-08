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
  auto new_wallet = wallet_functions.pub_key_to_addr(pub_key, mainnet);
  std::cout << "Wallet Address (Mainnet):      " << new_wallet.encoded()
            << std::endl;
  std::cout << "Wallet cashAddress (Mainnet):  " << new_wallet.encoded_cashaddr()
            << std::endl
            << std::endl;

  // Creates a payment_address using a cashAddr string

  std::string mainnet_address = new_wallet.encoded_cashaddr();
  new_wallet = wallet_functions.cashAddr_to_payment_address(mainnet_address, mainnet);
  std::cout << "Wallet Address (Imported):     " << new_wallet.encoded()
            << std::endl;
  std::cout << "Wallet cashAddress (Imported): " << new_wallet.encoded_cashaddr()
            << std::endl
            << std::endl;

  // Creates a tesnet wallet using the public key.
  mainnet = false;
  new_wallet = wallet_functions.pub_key_to_addr(pub_key, mainnet);
  std::cout << "Wallet Address (Testnet):      " << new_wallet.encoded()
            << std::endl;
  std::cout << "Wallet cashAddress (Testnet):  " << new_wallet.encoded_cashaddr()
            << std::endl
            << std::endl;

  // Creates a payment_address using a cashAddr string
  std::string testnet_address = new_wallet.encoded_cashaddr();
  new_wallet = wallet_functions.cashAddr_to_payment_address(testnet_address, mainnet);
  std::cout << "Wallet Address (Imported):     " << new_wallet.encoded()
            << std::endl;
  std::cout << "Wallet cashAddress (Imported): " << new_wallet.encoded_cashaddr()
            << std::endl
            << std::endl;

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

} //end namespace bitprim

int main() {
  std::cout << "Wallet functions: " << std::endl;
  bitprim::generate_addrs();
  std::cout << "Sending curls" << std::endl;
  bitprim::send_curl();


  // TODO: make the tx example using the output of the previous function
  std::cout << "Creating txn" << std::endl;
  bitprim::bitprim_transaction transaction;
  std::vector<std::string> inputs;
  inputs.push_back("980de6ce12c29698d54323c6b0f358e1a9ae867598b840ee0094b9df22b07393:1");
  std::vector<std::string> outputs;
  outputs.push_back("mwx2YDHgpdfHUmCpFjEi9LarXf7EkQN6YG:199999000");
  std::cout << transaction.tx_encode(inputs, outputs) << std::endl;

  std::cout << "Signing txn" << std::endl;
  std::cout << transaction.input_signature("8b1e51a4ec92844625db3e7a99444939243907ad3480e54622b9109c17693be9",
                                           "dup hash160 [b43ff4532569a00bcab4ce60f87cdeebf985b69a] equalverify checksig",
                                           "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000",
                                           200000000)
            << std::endl;

  std::cout << "Adding the signature to the txn" << std::endl;


  std::cout << transaction.input_set(
      "30440220433c405e4cb7698ad5f58e0ea162c3c3571d46d96ff1b3cb9232a06eba3b444d02204bc5f48647c0f052ade7cf85eac3911f7afbfa69fa5ebd92084191a5da33f88d41",
      "027a45d4abb6ebb00214796e2c7cf61d18c9185ba771fe9ed75b303eb7a8e9028b",
      "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000")
            << std::endl;

  return 0;
}

