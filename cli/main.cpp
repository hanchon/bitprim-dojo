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
      {"seed_to_wif",process_seed_to_wif},
      {"seed_to_pub", process_seed_to_pub},
      {"seed_to_wallet",process_seed_to_wallet},
      {"seed_to_cashaddr",process_seed_to_cashaddr},
      /* RPC functions */
      {"getinfo", process_getinfo},
      {"getaddresstxids",process_getaddresstxids}

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