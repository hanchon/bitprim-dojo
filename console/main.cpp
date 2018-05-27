//
// Created by hanchon on 27/05/18.
//

#include <iostream>

#include <wallet/wallet_functions.hpp>
#include <simple_web_client/requester.hpp>

namespace bitprim {

void generate_addrs() {
  // Try the wallet functions

  bitprim_wallet wallet_functions;

  // The seed must be base 16 encoded and at least 32 char long.
  std::string priv_key_seed = "aaeb587496cc54912bbcef874fa9a61c";

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
  return 0;
}

