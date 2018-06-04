//
// Created by hanchon on 6/3/18.
//

#ifndef BITPRIM_DOJO_PROCESS_REQUESTS_HPP
#define BITPRIM_DOJO_PROCESS_REQUESTS_HPP

#include <string>
#include <bitcoin/bitcoin.hpp>
#include <wallet/wallet_functions.hpp>
#include <simple_web_client/requester.hpp>

namespace bitprim {
// CONSTS
// TODO: set this value using a configuration file
static const std::string SERVER = "localhost:18332";


// Utis:
bool char_to_bool(char *in, bool &out) {
  // char can only be "true" or "false"
  std::stringstream ss(in);
  if (!(ss >> std::boolalpha >> out)) {
    return false;
  }
  return true;
}

//TODO: validate paramertes types and count
//TODO: split this file in subclases

/************** WALLET FUNCTIONS **************/
std::string process_priv_key(char *argv[]) {
  // example: priv_key aaeb587496cc54912bbcef874fa9a61c
  bitprim_wallet wallet_functions;
  auto priv_key = wallet_functions.generate_priv_key(argv[2]);
  return libbitcoin::encode_base16(priv_key);
}

std::string process_seed_to_wif(char *argv[]) {
  // example: seed_to_wif aaeb587496cc54912bbcef874fa9a61c true
  // the bool parameter is mainnet/testnet (mainnet = true)
  bitprim_wallet wallet_functions;
  auto priv_key = wallet_functions.generate_priv_key(argv[2]);
  bool mainnet;
  if (!char_to_bool(argv[3], mainnet)) {
    return "Bool parsing error";
  }
  return wallet_functions.priv_key_to_wif(priv_key, mainnet);
}

std::string process_seed_to_pub(char *argv[]) {
  // example: seed_to_pub aaeb587496cc54912bbcef874fa9a61c true
  // the bool parameter is compressed or not (compressed = true)
  bitprim_wallet wallet_functions;
  auto priv_key = wallet_functions.generate_priv_key(argv[2]);
  bool compress;
  if (!char_to_bool(argv[3], compress)) {
    return "Bool parsing error";
  }
  return wallet_functions.priv_key_to_public(priv_key, compress).encoded();
}

std::string process_seed_to_wallet(char *argv[]) {
  // example: seed_to_wallet aaeb587496cc54912bbcef874fa9a61c true true
  // first bool parameter is compressed (compressed = true)
  // second bool parameter is mainnet/testnet (mainnet = true)
  bitprim_wallet wallet_functions;
  auto priv_key = wallet_functions.generate_priv_key(argv[2]);
  bool compress;
  if (!char_to_bool(argv[3], compress)) {
    return "Bool parsing error";
  }
  auto pub_key = wallet_functions.priv_key_to_public(priv_key, compress).encoded();
  bool mainnet;
  if (!char_to_bool(argv[4], mainnet)) {
    return "Bool parsing error";
  }
  auto new_wallet = wallet_functions.pub_key_to_addr(pub_key, mainnet);
  return new_wallet.encoded();
}

std::string process_seed_to_cashaddr(char *argv[]) {
  // example: seed_to_cashaddr aaeb587496cc54912bbcef874fa9a61c true true
  // first bool parameter is compressed (compressed = true)
  // second bool parameter is mainnet/testnet (mainnet = true)
  bitprim_wallet wallet_functions;
  auto priv_key = wallet_functions.generate_priv_key(argv[2]);
  bool compress;
  if (!char_to_bool(argv[3], compress)) {
    return "Bool parsing error";
  }
  auto pub_key = wallet_functions.priv_key_to_public(priv_key, compress).encoded();
  bool mainnet;
  if (!char_to_bool(argv[4], mainnet)) {
    return "Bool parsing error";
  }
  auto new_wallet = wallet_functions.pub_key_to_addr(pub_key, mainnet);
  return new_wallet.encoded_cashaddr();
}

/************** RPC FUNCTIONS **************/
std::string process_getinfo(char *argv[]) {
  // example: getinfo
  http_requester requester(SERVER);
  std::string query = requester.create_query("getinfo", "\"\"");
  auto reply = requester.request(query);
  if (reply.first) {
    return reply.second;
  } else {
    return "RPC call failed";
  }
}

std::string process_getaddresstxids(char *argv[]) {
  // example: getaddresstxids, "[{\"addresses\": [\"n3GNqMveyvaPvUbH469vDRadqpJMPc84JA\"]}]"
  http_requester requester(SERVER);
  std::string query = requester.create_query("getaddresstxids", argv[2]);
  auto reply = requester.request(query);
  if (reply.first) {
    return reply.second;
  } else {
    return "RPC call failed";
  }
}

// TODO: add all the bitprim RPC calls, refactor the code and just change the query

/************** TRANSACTION FUNCTIONS **************/
// TODO: add the create tx, add input and sign txn functions
}// end namespace bitprim

#endif //BITPRIM_DOJO_PROCESS_REQUESTS_HPP
