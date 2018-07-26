//
// Created by hanchon on 25/07/18.
//

#include <simple_web_server/server_http.hpp>
#include <simple_web_server/json.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/chain/input_point.hpp>
#include <bitprim/keoken/message/base.hpp>
#include <bitprim/keoken/wallet/createtransaction.hpp>


using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

//libbitcoin::chain::transaction create_assets(std::string& name,
//                                              bitprim::keoken::message::amount_t amount_tokens,
//                                              libbitcoin::chain::input_point const& output_to_spend,
//                                              libbitcoin::wallet::payment_address address,
//                                              size_t value) {
//
//  return bitprim::keoken::wallet::tx_encode_create_asset({output_to_spend}, {address, value}, name, amount_tokens).second;
//
//}


libbitcoin::ec_secret create_secret_from_seed(std::string const& seed_str) {
  libbitcoin::data_chunk seed;
  libbitcoin::decode_base16(seed, seed_str);
  libbitcoin::wallet::hd_private const key(seed);
  // Secret key
  libbitcoin::ec_secret secret_key(key.secret());
  return secret_key;
}

libbitcoin::wallet::ec_public secret_to_compressed_public(libbitcoin::ec_secret const& secret_key) {
  //Public key
  libbitcoin::ec_compressed point;
  libbitcoin::secret_to_public(point, secret_key);
  libbitcoin::wallet::ec_public public_key(point, true /*compress*/);

  return public_key;
}

std::string create_tx(nlohmann::json const& json_object) {
  std::vector<libbitcoin::chain::input_point> outputs_to_spend;
  std::vector<std::pair<libbitcoin::wallet::payment_address, uint64_t>> outputs;
  libbitcoin::chain::output::list extra_outputs;

  libbitcoin::hash_digest hash_to_spend;
  for (auto const& o : json_object["origin"]) {
    libbitcoin::decode_hash(hash_to_spend, o["output_hash"]);
    outputs_to_spend.push_back({hash_to_spend, o["output_index"]});
  }

  for (auto const& d : json_object["dests"]) {
    // Implicit json to string conversion
    std::string addr = d["addr"];
    outputs.push_back({libbitcoin::wallet::payment_address(addr), d["amount"]});
  }

  for (auto const& extra:json_object["extra_outputs"]){
    libbitcoin::data_chunk script_string;
    libbitcoin::decode_base16(script_string, extra["script"]);
    libbitcoin::chain::script script;
    script.from_data(script_string, false);
    extra_outputs.push_back({extra["amount"], script});
  }

  return libbitcoin::encode_base16(tx_encode(outputs_to_spend, outputs, extra_outputs).second.to_data(true));
}

std::string create_signature(nlohmann::json const& json_object) {
  // Priv key
  auto const private_key = create_secret_from_seed(json_object["seed"]);
  // Script
  libbitcoin::data_chunk raw_script;
  libbitcoin::decode_base16(raw_script, json_object["script"]);
  libbitcoin::chain::script output_script(raw_script, false);
  // TX
  libbitcoin::chain::transaction tx;
  libbitcoin::data_chunk raw_data;
  libbitcoin::decode_base16(raw_data, json_object["tx"]);
  tx.from_data(raw_data);
  // Amount
  uint64_t const amount = json_object["amount"];
  // Index
  uint32_t const index = json_object["index"];

  return libbitcoin::encode_base16(libbitcoin::wallet::input_signature_bch(private_key, output_script, tx, amount, index).second);
}

std::string set_signature(nlohmann::json const& json_object) {
  // This only works for compressed public keys
  // Priv key
  auto const private_key = create_secret_from_seed(json_object["seed"]);
  // Pub key
  auto pub_key = secret_to_compressed_public(private_key);
  // Redeem script for P2KH [SIGNATURE][PUBKEY]
  libbitcoin::chain::script input_script;
  std::string json_signature  = json_object["signature"];
  input_script.from_string("[" + json_signature + "] [" + pub_key.encoded() + "]");
  // TX
  libbitcoin::chain::transaction tx;
  libbitcoin::data_chunk raw_data;
  libbitcoin::decode_base16(raw_data, json_object["tx"]);
  tx.from_data(raw_data);
  // Index
  uint32_t const index = json_object["index"];

  return libbitcoin::encode_base16(libbitcoin::wallet::input_set(input_script, tx, index).second.to_data(true));
}
// TODO: create the set_signature call without seed, reading from the json the input script complete

std::string process_data(nlohmann::json const& json_object) {
  if (json_object.find("method") != json_object.end()) {
    std::string method = *(json_object.find("method"));
    if (method == "createtx") {
      return create_tx(json_object["param"]);
    } else if (method == "create_signature") {
      return create_signature(json_object["param"]);
    } else if (method == "set_signature"){
      return set_signature(json_object["param"]);
    }
  }

  return "";
}


int main(int argc, char *argv[]) {
  unsigned short port = 3000;
  if (std::string(argv[1]) == "-port" || std::string(argv[1]) == "-p") {
      std::stringstream temp(argv[2]);
      temp >> port;
    } else {
      // Error: port requires one argument
      return 1;
    }

  HttpServer server;
  server.config.port = port;
  server.default_resource["POST"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
      try {
        auto json_str = request->content.string();
        if (json_str.size() > 0 && json_str.back() == '\n') {
          json_str.pop_back();
        }
        nlohmann::json json_object = nlohmann::json::parse(json_str);

        auto result = process_data(json_object);
        result = result + "\u000a";

        *response << "HTTP/1.1 200 OK\r\n"
                  << "Content-Type: application/json\r\n"
                  << "Content-Length: " << result.length() << "\r\n\r\n"
                  << result;

      } catch(std::exception const& e) {
        *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
      }
  };

  server.start();


  return 0;
}