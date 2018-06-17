//
// Created by hanchon on 27/05/18.
//

#ifndef BITPRIM_DOJO_WALLET_FUNCTIONS_HPP
#define BITPRIM_DOJO_WALLET_FUNCTIONS_HPP

#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/multi_crypto_support.hpp>
#include <bitcoin/bitcoin/wallet/ec_private.hpp>
#include <bitcoin/bitcoin/wallet/hd_private.hpp>
#include <bitcoin/bitcoin/wallet/payment_address.hpp>

namespace bitprim {
class bitprim_wallet {
private:
  /**
* The minimum safe length of a seed in bits (multiple of 8).
*/
  const size_t minimum_seed_bits = 128;

/**
 * The minimum safe length of a seed in bytes (16).
 */
  const size_t minimum_seed_size = minimum_seed_bits / libbitcoin::byte_bits;

public:
  // This function generates a private key using the seed_data string.
  libbitcoin::ec_secret generate_priv_key(std::string const &seed_data);

  // This function generates a public key using a private key. The public key
  // can be compressed or uncompressed.
  libbitcoin::wallet::ec_public
  priv_key_to_public(libbitcoin::ec_secret const &priv_key,
                     bool compress = false);

  // This function generates a payment address using the public key. The payment
  // address can be for mainnet or testnet.
  libbitcoin::wallet::payment_address
  pub_key_to_addr(libbitcoin::wallet::ec_public const &pub_key,
                  bool mainnet = true);

  // Encode a private key to WIF
  std::string priv_key_to_wif(libbitcoin::ec_secret const &priv_key,
                              bool mainnet = true, bool compress = false);

#ifdef BITPRIM_CURRENCY_BCH
  // Creates a payment_address using a cashAddr string
  libbitcoin::wallet::payment_address
  cashAddr_to_payment_address(std::string const &address, bool mainnet = true);
#endif

  // WARNING: Pseudorandom seeding can introduce cryptographic weakness into your keys.
  // This command is provided as a convenience.
  std::string new_seed();

  //TODO: add method for mnemonics functions like mnemonic-new, mnemonic-to-seed, hd-new

};
} //end namespace bitprim

#endif //BITPRIM_DOJO_WALLET_FUNCTIONS_HPP