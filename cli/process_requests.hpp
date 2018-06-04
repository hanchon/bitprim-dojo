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
std::string process_rpc_call(std::string method, std::string params) {
  http_requester requester(SERVER);
  std::string query = requester.create_query(method, params);
  auto reply = requester.request(query);
  if (reply.first) {
    return reply.second;
  } else {
    return "RPC call failed";
  }

}
std::string process_getinfo(char *argv[]) {
  // example: getinfo
  return process_rpc_call("getinfo", "\"\"");
}

std::string process_getrawtransaction (char *argv[]) {
  return process_rpc_call("getrawtransaction", argv[2]);
}

std::string process_getaddressbalance (char *argv[]) {
  return process_rpc_call("getaddressbalance", argv[2]);
}

std::string process_getspentinfo (char *argv[]) {
  return process_rpc_call("getspentinfo", argv[2]);
}

std::string process_getaddresstxids(char *argv[]) {
  // example: getaddresstxids, "[{\"addresses\": [\"n3GNqMveyvaPvUbH469vDRadqpJMPc84JA\"]}]"
  return process_rpc_call("getaddresstxids", argv[2]);
}

std::string process_getaddressdeltas(char *argv[]) {
  return process_rpc_call("getaddressdeltas", argv[2]);
}

std::string process_getaddressutxos(char *argv[]) {
  return process_rpc_call("getaddressutxos", argv[2]);
}

std::string process_getblockhashes(char *argv[]) {
  return process_rpc_call("getblockhashes", argv[2]);
}

std::string process_getaddressmempool(char *argv[]) {
  return process_rpc_call("getaddressmempool", argv[2]);
}

std::string process_getbestblockhash(char *argv[]) {
  return process_rpc_call("getbestblockhash", argv[2]);
}

std::string process_getblock(char *argv[]) {
  return process_rpc_call("getblock", argv[2]);
}

std::string process_getblockhash(char *argv[]) {
  return process_rpc_call("getblockhash", argv[2]);
}

std::string process_getblockchaininfo(char *argv[]) {
  return process_rpc_call("getblockchaininfo", argv[2]);
}

std::string process_getblockheader(char *argv[]) {
  return process_rpc_call("getblockheader", argv[2]);
}

std::string process_getblockcount(char *argv[]) {
  return process_rpc_call("getblockcount", argv[2]);
}

std::string process_getdifficulty(char *argv[]) {
  return process_rpc_call("getdifficulty", argv[2]);
}

std::string process_getchaintips(char *argv[]) {
  return process_rpc_call("getchaintips", argv[2]);
}

std::string process_validateaddress(char *argv[]) {
  return process_rpc_call("validateaddress", argv[2]);
}

std::string process_getblocktemplate(char *argv[]) {
  return process_rpc_call("getblocktemplate", argv[2]);
}

std::string process_getmininginfo(char *argv[]) {
  return process_rpc_call("getmininginfo", argv[2]);
}

std::string process_submitblock(char *argv[]) {
  return process_rpc_call("submitblock", argv[2]);
}

std::string process_sendrawtransaction(char *argv[]) {
  return process_rpc_call("sendrawtransaction", argv[2]);
}

/************** TRANSACTION FUNCTIONS **************/
// TODO: add the create tx, add input and sign txn functions

}// end namespace bitprim

#endif //BITPRIM_DOJO_PROCESS_REQUESTS_HPP
