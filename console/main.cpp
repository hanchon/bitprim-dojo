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


  // TODO: make the tx example using the output of the previous function
  std::cout << "Creating txn" << std::endl;
  bitprim::bitprim_transaction transaction;
  std::vector<std::string> inputs;
  inputs.push_back("97e06e49dfdd26c5a904670971ccf4c7fe7d9da53cb379bf9b442fc9427080b3:0");
  std::vector<std::string> outputs;
  outputs.push_back("1966U1pjj15tLxPXZ19U48c99EJDkdXeqb:45000");
  std::cout << transaction.tx_encode(inputs, outputs) << std::endl;

  std::cout << "Signing txn" << std::endl;
  std::cout << transaction.input_signature("4ce3eb6bd06c224e3c355352a488720efc5ac9fe527a219ad35178c3cf762350",
                                           "dup hash160 [c564c740c6900b93afc9f1bdaef0a9d466adf6ee] equalverify checksig",
                                           "01000000017d01943c40b7f3d8a00a2d62fa1d560bf739a2368c180615b0a7937c0e883e7c0000000000ffffffff01c8af0000000000001976a91458b7a60f11a904feef35a639b6048de8dd4d9f1c88ac00000000")
            << std::endl;

  std::cout << "Adding the signature to the txn" << std::endl;

////   TODO: public key is generated using ec-to-public 4ce3eb6bd06c224e3c355352a488720efc5ac9fe527a219ad35178c3cf762350
////  bitprim::bitprim_wallet wallet_functions;
////  libbitcoin::ec_secret a;
////  libbitcoin::decode_base16(a, "4ce3eb6bd06c224e3c355352a488720efc5ac9fe527a219ad35178c3cf762350");
////  auto pub_key = wallet_functions.priv_key_to_public(a, true);
////  std::cout << pub_key << std::endl;

  std::cout << transaction.input_set(
      "304402206f6a28f4f8c94b749164425c4ca3c0f400b6e4abb86c08094025b559aff79ad90220727276b10633436b96aee3abeea3826975920e99d09f4325891499f037e1b24e01",
      "03e208f5403383c77d5832a268c9f71480f6e7bfbdfa44904becacfad66163ea31",
      "0100000001b3807042c92f449bbf79b33ca59d7dfec7f4cc71096704a9c526dddf496ee0970000000000ffffffff01c8af0000000000001976a91458b7a60f11a904feef35a639b6048de8dd4d9f1c88ac00000000")
            << std::endl;

  return 0;
}

