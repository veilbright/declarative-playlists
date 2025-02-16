#include "crypto_util.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <vector>

// Expects a randomly generated unsigned char, returns a valid char for a PKCE
// code verifier
void CryptoUtil::MapForVerifier(unsigned char &c) { c = kPkceChars[c]; }

// Generates a random vector of chars consisting of a-Z, 0-9, _.-~
// Returns an empty vector if length is not between 43 - 128
std::vector<unsigned char> CryptoUtil::GenPkceVerifier(std::size_t length) {
  if (length < 43 || length > 128) {
    return {};
  }
  std::vector<unsigned char> buf;
  buf.resize(length);
  if (RAND_bytes(buf.data(), length) != 1) {
    std::cerr << "ERROR: Failed to generate random string.";
    std::exit(EXIT_FAILURE);
  }
  std::for_each(buf.begin(), buf.end(), MapForVerifier);
  return buf;
}

std::array<unsigned char, 32>
CryptoUtil::Sha256(const std::vector<unsigned char> &data) {
  std::array<unsigned char, 32> cArr; // 32 is size of SHA256 output
  SHA256(data.data(), data.size(), cArr.data());
  return cArr;
}

// Based off RFC 4648 Section 4
std::vector<unsigned char>
CryptoUtil::Base64Encode(const std::vector<unsigned char> &data) {
  std::vector<unsigned char> encoded;

  // total size will be data.size * 4/3, rounded up to divisible of 4
  encoded.reserve((data.size() * 4) / 3);

  int i;
  // iterate 3 chars at a time to encode
  for (i = 0; i < data.size() - 2; i += 3) {
    unsigned char c1 = data[i];
    unsigned char c2 = data[i + 1];
    unsigned char c3 = data[i + 2];
    encoded.push_back(kBase64[c1 >> 2]);
    encoded.push_back(kBase64[((c1 & 0x03) << 4) | (c2 >> 4)]);
    encoded.push_back(kBase64[((c2 & 0x0F) << 2) | (c3 >> 6)]);
    encoded.push_back(kBase64[c3 & 0x3F]);
  }
  // fill in remaining chars (if any)
  if (data.size() - i == 1) { // missed 1 char
    unsigned char c1 = *data.rbegin();
    encoded.push_back(kBase64[c1 >> 2]);
    encoded.push_back(kBase64[(c1 & 0x03) << 6]);
  } else if (data.size() - i == 2) { // missed 2 chars
    unsigned char c1 = data[data.size() - 2];
    unsigned char c2 = data[data.size() - 1];
    encoded.push_back(kBase64[c1 >> 2]);
    encoded.push_back(kBase64[((c1 & 0x03) << 6) | ((c2 & 0xF0) >> 4)]);
    encoded.push_back(kBase64[(c2 & 0x0F) << 2]);
  }
  // pad ending of encoding with '=' (needs to be divisible of 4)
  while (encoded.size() % 4 != 0) {
    encoded.push_back('=');
  }
  return encoded;
}
