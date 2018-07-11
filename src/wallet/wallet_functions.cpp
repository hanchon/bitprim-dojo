//
// Created by hanchon on 27/05/18.
//

#include <wallet/wallet_functions.hpp>
#include <bitcoin/bitcoin.hpp>

namespace bitprim {

// The seed must be base 16 encoded and at least 32 char long.
libbitcoin::ec_secret bitprim_wallet::generate_priv_key(std::string const &seed_data) {
  // This function generates a private key using the seed_data string.

  // Bound parameters.
  libbitcoin::data_chunk seed;
  if (!libbitcoin::decode_base16(seed, seed_data)) {
    // Seed is not base16
    return {};
  }

  if (seed.size() < minimum_seed_size) {
    // Short seed
    return {};
  }

  const libbitcoin::wallet::hd_private key(seed);
  libbitcoin::ec_secret secret(key.secret());

  if (secret == libbitcoin::null_hash) {
    // New invalid key
    return {};
  }

  return secret;
}

libbitcoin::wallet::ec_public
bitprim_wallet::priv_key_to_public(libbitcoin::ec_secret const &priv_key,
                                   bool compress /*= false*/) {
  // This function generates a public key using a private key. The public key
  // can be compressed or uncompressed.

  libbitcoin::ec_compressed point;
  libbitcoin::secret_to_public(point, priv_key);
  return libbitcoin::wallet::ec_public(point, compress);
}

libbitcoin::wallet::payment_address
bitprim_wallet::pub_key_to_addr(libbitcoin::wallet::ec_public const &pub_key,
                                bool mainnet /*= true*/) {
  // This function generates a payment address using the public key. The payment
  // address can be for mainnet or testnet.
  // The payment_address knows how to encode itself as cashAddr
  // (encoded_cashaddr()) and legacy(encoded())

  uint8_t version;
  if (mainnet) {
    version = libbitcoin::wallet::payment_address::mainnet_p2kh; // mainnet_p2sh
    // can also be
    // used
  } else {
    version = libbitcoin::wallet::payment_address::testnet_p2kh; // testnet_p2sh
    // can also be
    // used
  }
  return libbitcoin::wallet::payment_address(pub_key, version);
}

std::string bitprim_wallet::priv_key_to_wif(libbitcoin::ec_secret const &priv_key,
                                            bool mainnet /*= true*/, bool compress /*= false*/) {
  // Encode a private key to WIF

  uint8_t wif_version = 128;
  if (!mainnet) {
    wif_version = 239;
  }
  // Payment address version is unused in this scenario.
  const uint8_t payment_version = 0;
  // ec_private requires a composite version.
  const auto version =
      libbitcoin::wallet::ec_private::to_version(payment_version, wif_version);

  libbitcoin::wallet::ec_private wif(priv_key, version, compress);

  return wif.encoded();
}
#ifdef BITPRIM_CURRENCY_BCH
// Creates a payment_address using a cashAddr string
libbitcoin::wallet::payment_address
bitprim_wallet::cashAddr_to_payment_address(std::string const &address, bool mainnet /*= true*/) {
  // Encode a private key to WIF

  libbitcoin::set_cashaddr_prefix("bitcoincash");
  if (!mainnet) {
    libbitcoin::set_cashaddr_prefix("bchtest");
  }
  const libbitcoin::wallet::payment_address new_address(address);
  return new_address;
}
#endif

// WARNING: Pseudorandom seeding can introduce cryptographic weakness into your keys.
// This command is provided as a convenience.
std::string bitprim_wallet::new_seed() {
  size_t bit_length = 192;
  size_t fill_seed_size = bit_length / libbitcoin::byte_bits;
  libbitcoin::data_chunk seed(fill_seed_size);
  libbitcoin::pseudo_random_fill(seed);
  return libbitcoin::encode_base16(seed);
}

} // end namespace bitprim