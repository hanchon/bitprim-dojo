//
// Created by hanchon on 6/1/18.
//

#include <iostream>
#include "process_requests.hpp"

namespace bitprim {
using request_signature = std::string(*)(char *argv[]);
using request_map = std::unordered_map<std::string, request_signature>;

request_map load_request_map() {

  return request_map{
      /* Wallet functions */
      {"priv_key", process_priv_key},
      {"seed_to_wif", process_seed_to_wif},
      {"seed_to_pub", process_seed_to_pub},
      {"seed_to_wallet", process_seed_to_wallet},
      {"seed_to_cashaddr", process_seed_to_cashaddr},
      /* RPC functions */
      {"getinfo", process_getinfo},
      {"getrawtransaction", process_getrawtransaction},
      {"getaddressbalance", process_getaddressbalance},
      {"getspentinfo", process_getspentinfo},
      {"getaddresstxids", process_getaddresstxids},
      {"getaddressdeltas", process_getaddressdeltas},
      {"getaddressutxos", process_getaddressutxos},
      {"getblockhashes", process_getblockhashes},
      {"getaddressmempool", process_getaddressmempool},
      {"getbestblockhash", process_getbestblockhash},
      {"getblock", process_getblock},
      {"getblockhash", process_getblockhash},
      {"getblockchaininfo", process_getblockchaininfo},
      {"getblockheader", process_getblockheader},
      {"getblockcount", process_getblockcount},
      {"getdifficulty", process_getdifficulty},
      {"getchaintips", process_getchaintips},
      {"validateaddress", process_validateaddress},
      {"getblocktemplate", process_getblocktemplate},
      {"getmininginfo", process_getmininginfo},
      {"submitblock", process_submitblock},
      {"sendrawtransaction", process_sendrawtransaction}

  };
}
} // end namespace bitprim

int main(int argc, char *argv[]) {
  const auto functions = bitprim::load_request_map();
  std::string function = argv[1];

  auto it = functions.find(function);

  if (it != functions.end()) {
    std::cout << it->second(argv) << std::endl;
  } else {
    std::cout << "Function not found." << std::endl;
  }

  return 0;
}