//
// Created by hanchon on 6/3/18.
//

#ifndef BITPRIM_DOJO_PROCESS_REQUESTS_HPP
#define BITPRIM_DOJO_PROCESS_REQUESTS_HPP

#include <string>
#include <bitcoin/bitcoin.hpp>
#include <wallet/wallet_functions.hpp>
#include <simple_web_client/requester.hpp>
#include <wallet/transaction_functions.hpp>

namespace bitprim {
// CONSTS
// TODO: set this value using a configuration file
static const std::string SERVER = "localhost:18332";

// Utis:
bool char_to_bool(char *in, bool &out) {
  // char * can only be "true" or "false"
  std::stringstream ss(in);
  if (!(ss >> std::boolalpha >> out)) {
    return false;
  }
  return true;
}

std::vector<std::string> comma_delimited_to_array(char *argv) {
  std::vector<std::string> result;
  std::stringstream raw(argv);
  while (raw.good()) {
    std::string substr;
    getline(raw, substr, ',');
    result.push_back(substr);
  }
  return result;
}

// TODO: validate parameters types and count
// TODO: split this file in subclasses

/************** WALLET FUNCTIONS **************/
std::string process_priv_key(int argc, char *argv[]) {
  // example: priv_key aaeb587496cc54912bbcef874fa9a61c
  bitprim_wallet wallet_functions;
  auto priv_key = wallet_functions.generate_priv_key(argv[2]);
  return libbitcoin::encode_base16(priv_key);
}

std::string process_seed_to_wif(int argc, char *argv[]) {
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

std::string process_seed_to_pub(int argc, char *argv[]) {
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

std::string process_seed_to_wallet(int argc, char *argv[]) {
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

#ifdef BITPRIM_CURRENCY_BCH
std::string process_seed_to_cashaddr(int argc, char *argv[]) {
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
#endif

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

std::string process_getinfo(int argc, char *argv[]) {
  // example: getinfo
  return process_rpc_call("getinfo", "\"\"");
}

std::string process_getrawtransaction(int argc, char *argv[]) {
  return process_rpc_call("getrawtransaction", argv[2]);
}

std::string process_getaddressbalance(int argc, char *argv[]) {
  return process_rpc_call("getaddressbalance", argv[2]);
}

std::string process_getspentinfo(int argc, char *argv[]) {
  return process_rpc_call("getspentinfo", argv[2]);
}

std::string process_getaddresstxids(int argc, char *argv[]) {
  // example: getaddresstxids, "[{\"addresses\": [\"n3GNqMveyvaPvUbH469vDRadqpJMPc84JA\"]}]"
  return process_rpc_call("getaddresstxids", argv[2]);
}

std::string process_getaddressdeltas(int argc, char *argv[]) {
  return process_rpc_call("getaddressdeltas", argv[2]);
}

std::string process_getaddressutxos(int argc, char *argv[]) {
  return process_rpc_call("getaddressutxos", argv[2]);
}

std::string process_getblockhashes(int argc, char *argv[]) {
  return process_rpc_call("getblockhashes", argv[2]);
}

std::string process_getaddressmempool(int argc, char *argv[]) {
  return process_rpc_call("getaddressmempool", argv[2]);
}

std::string process_getbestblockhash(int argc, char *argv[]) {
  return process_rpc_call("getbestblockhash", argv[2]);
}

std::string process_getblock(int argc, char *argv[]) {
  return process_rpc_call("getblock", argv[2]);
}

std::string process_getblockhash(int argc, char *argv[]) {
  return process_rpc_call("getblockhash", argv[2]);
}

std::string process_getblockchaininfo(int argc, char *argv[]) {
  return process_rpc_call("getblockchaininfo", argv[2]);
}

std::string process_getblockheader(int argc, char *argv[]) {
  return process_rpc_call("getblockheader", argv[2]);
}

std::string process_getblockcount(int argc, char *argv[]) {
  return process_rpc_call("getblockcount", argv[2]);
}

std::string process_getdifficulty(int argc, char *argv[]) {
  return process_rpc_call("getdifficulty", argv[2]);
}

std::string process_getchaintips(int argc, char *argv[]) {
  return process_rpc_call("getchaintips", argv[2]);
}

std::string process_validateaddress(int argc, char *argv[]) {
  return process_rpc_call("validateaddress", argv[2]);
}

std::string process_getblocktemplate(int argc, char *argv[]) {
  return process_rpc_call("getblocktemplate", argv[2]);
}

std::string process_getmininginfo(int argc, char *argv[]) {
  return process_rpc_call("getmininginfo", argv[2]);
}

std::string process_submitblock(int argc, char *argv[]) {
  return process_rpc_call("submitblock", argv[2]);
}

std::string process_sendrawtransaction(int argc, char *argv[]) {
  return process_rpc_call("sendrawtransaction", argv[2]);
}

/************** TRANSACTION FUNCTIONS **************/
std::string process_create_txn(int argc, char *argv[]) {
  // example: create_txn -i "980de6ce12c29698d54323c6b0f358e1a9ae867598b840ee0094b9df22b07393:1" -o "mwx2YDHgpdfHUmCpFjEi9LarXf7EkQN6YG:199999000"
  std::vector<std::string> inputs;
  std::vector<std::string> outputs;

  // Parse values
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-i") {
      if (i + 1 < argc) {
        inputs = comma_delimited_to_array(argv[++i]);
      } else {
        return "Error, inputs requires one argument";
      }
    } else {
      if (std::string(argv[i]) == "-o") {
        if (i + 1 < argc) {
          outputs = comma_delimited_to_array(argv[++i]);
        } else {
          return "Error, outputs requires one argument";
        }
      }
    }
  }

  // Create txn
  bitprim::bitprim_transaction transaction;
  return transaction.tx_encode(inputs, outputs);
}

std::string process_create_signature(int argc, char *argv[]) {
  // example: create_signature
  //            -seed "fffb587496cc54912bbcef874fa9a61a"
  //            -script "dup hash160 [b43ff4532569a00bcab4ce60f87cdeebf985b69a] equalverify checksig"
  //            -amount "200000000"
  //            -txn "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000"

  // TODO: add index, by default it signs the input 0

  std::string seed;
  std::string script;
  uint64_t amount;
  std::string txn;

  // Parse values
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-seed") {
      if (i + 1 < argc) {
        seed = argv[++i];
      } else {
        return "Error, seed requires one argument";
      }
    } else if (std::string(argv[i]) == "-script") {
      if (i + 1 < argc) {
        script = argv[++i];
      } else {
        return "Error, script requires one argument";
      }
    } else if (std::string(argv[i]) == "-txn") {
      if (i + 1 < argc) {
        txn = argv[++i];
      } else {
        return "Error, txn requires one argument";
      }
    } else if (std::string(argv[i]) == "-amount") {
      if (i + 1 < argc) {
        std::stringstream temp(argv[++i]);
        temp >> amount;
      } else {
        return "Error, amount requires one argument";
      }
    }
  }

  // Seed to private
  bitprim::bitprim_wallet wallet_functions;
  const auto priv_key = wallet_functions.generate_priv_key(seed);
  const auto priv_key_string = libbitcoin::encode_base16(priv_key);

  // Sign the txn
  bitprim::bitprim_transaction transaction;
  return transaction.input_signature(priv_key_string, script, txn, amount);

}

std::string process_set_input_script(int argc, char *argv[]) {
  // example: set_input_script
  //            -seed "fffb587496cc54912bbcef874fa9a61a"
  //            -signature "30440220433c405e4cb7698ad5f58e0ea162c3c3571d46d96ff1b3cb9232a06eba3b444d02204bc5f48647c0f052ade7cf85eac3911f7afbfa69fa5ebd92084191a5da33f88d41"
  //            -txn "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000"

  // TODO: add index, by default it sets the input 0
  // TODO: create a new function that accepts pub_key instead of seed in case the user wants privacy
  // TODO: the current code only set compressed public keys (created from the seed), it can be parametrized to support the no-compressed format

  std::string seed;
  std::string signature;
  std::string txn;

  // Parse values
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-seed") {
      if (i + 1 < argc) {
        seed = argv[++i];
      } else {
        return "Error, seed requires one argument";
      }
    } else if (std::string(argv[i]) == "-signature") {
      if (i + 1 < argc) {
        signature = argv[++i];
      } else {
        return "Error, signature requires one argument";
      }
    } else if (std::string(argv[i]) == "-txn") {
      if (i + 1 < argc) {
        txn = argv[++i];
      } else {
        return "Error, txn requires one argument";
      }
    }
  }

  // Seed to private
  bitprim::bitprim_wallet wallet_functions;
  const auto priv_key = wallet_functions.generate_priv_key(seed);
  // Private to public
  auto pub_key = wallet_functions.priv_key_to_public(priv_key, true);

  // Set the script
  bitprim::bitprim_transaction transaction;
  return transaction.input_set(signature, pub_key.encoded(), txn);
}

}// end namespace bitprim

#endif //BITPRIM_DOJO_PROCESS_REQUESTS_HPP
