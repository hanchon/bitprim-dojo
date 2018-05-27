//
// Created by hanchon on 27/05/18.
//

#include <simple_web_client/requester.hpp>

namespace bitprim {

std::pair<bool, std::string> http_requester::request(const std::string &query) {
  //Query example =  "{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"getinfo\", \"params\": \"\" }";
  //Query example =  "{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"getaddresstxids\", \"params\": [{\"addresses\": [\"mkR6TH68C5DXgjQDn7eDjTRFwNvo5V5zdA\"]}] }";

  HttpClient client(endpoint_);
  // Synchronous request
  try {
    auto r2 = client.request("POST", "/string", query);
    std::ostringstream ss;
    ss << r2->content.rdbuf();
    return std::make_pair(true, ss.str());
  }
  catch (const SimpleWeb::system_error &e) {
    return std::make_pair(false, e.what());
  }
}

std::string http_requester::create_query(const std::string &method, const std::string &params) {
  //Method =  getaddresstxids
  //Params [{\"addresses\": [\"mkR6TH68C5DXgjQDn7eDjTRFwNvo5V5zdA\"]}]
  std::string ret =
      "{\"jsonrpc\": \"1.0\", \"id\":\"bitprimrequest\", \"method\": \"" + method + "\", \"params\": " + params + "}";
  return ret;
}

} //end namespace bitprim


