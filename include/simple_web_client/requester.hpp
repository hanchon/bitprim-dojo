//
// Created by hanchon on 27/05/18.
//

#ifndef BITPRIM_DOJO_REQUESTER_HPP
#define BITPRIM_DOJO_REQUESTER_HPP

#include <simple_web_client/client_http.hpp>

namespace bitprim {

using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;

class http_requester {
private:
  std::string endpoint_;
public:
  http_requester(std::string endpoint) :
      endpoint_(endpoint) {};

  std::pair<bool, std::string> request(const std::string &query);

  std::string create_query(const std::string &method, const std::string &params);
};
} // end namespace bitprim

#endif //BITPRIM_DOJO_REQUESTER_HPP
