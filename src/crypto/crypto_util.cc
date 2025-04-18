#include "crypto_util.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <vector>

// Generates a random string consisting of a-Z, 0-9
// Returns an empty vector if length is not between 43 - 128
// TODO: Throw when the length is wrong
std::string const CryptoUtil::GenPkceVerifier(std::size_t length) {
    if (length < 43 || length > 128) {
        return "";
    }
    std::vector<unsigned char> buf;
    buf.resize(length);
    if (RAND_bytes(buf.data(), length) != 1) {
        // TODO: LOG
        std::cerr << "ERROR: Failed to generate random string.";
        std::exit(EXIT_FAILURE);
    }
    std::transform(buf.begin(), buf.end(), buf.begin(),
                   [](unsigned char c) { return kPkceChars[c % std::size(kPkceChars)]; });
    return std::string(buf.begin(), buf.end());
}

std::array<unsigned char, 32> const CryptoUtil::Sha256(const std::string &str) {
    std::array<unsigned char, 32> c_arr; // 32 is size of SHA256 output
    SHA256(reinterpret_cast<const unsigned char *>(str.c_str()), str.length(), c_arr.data());
    return c_arr;
}

// Based off RFC 4648 Section 4
std::vector<unsigned char> const CryptoUtil::Base64Encode(const std::vector<unsigned char> &data) {
    if (data.empty()) {
        return {};
    }
    // total size will be data.size * 4/3, rounded up to divisible of 4
    size_t encoded_size = ((data.size() + 2) / 3) * 4;
    std::vector<unsigned char> encoded(encoded_size);

    EVP_EncodeBlock(encoded.data(), data.data(), data.size());
    return encoded;
}
