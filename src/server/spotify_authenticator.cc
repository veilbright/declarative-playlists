#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <vector>

const unsigned char kPkceChars[] = {
    'a', 'a', 'a', 'a', 'b', 'b', 'b', 'b', 'c', 'c', 'c', 'c', 'd', 'd', 'd',
    'd', 'e', 'e', 'e', 'e', 'f', 'f', 'f', 'f', 'g', 'g', 'g', 'g', 'h', 'h',
    'h', 'h', 'i', 'i', 'i', 'i', 'j', 'j', 'j', 'j', 'k', 'k', 'k', 'k', 'l',
    'l', 'l', 'l', 'm', 'm', 'm', 'm', 'n', 'n', 'n', 'n', 'o', 'o', 'o', 'o',
    'p', 'p', 'p', 'p', 'q', 'q', 'q', 'q', 'r', 'r', 'r', 'r', 's', 's', 's',
    's', 't', 't', 't', 't', 'u', 'u', 'u', 'u', 'v', 'v', 'v', 'v', 'w', 'w',
    'w', 'w', 'x', 'x', 'x', 'x', 'y', 'y', 'y', 'y', 'z', 'z', 'z', 'z', 'A',
    'A', 'A', 'A', 'B', 'B', 'B', 'B', 'C', 'C', 'C', 'C', 'D', 'D', 'D', 'D',
    'E', 'E', 'E', 'E', 'F', 'F', 'F', 'F', 'G', 'G', 'G', 'G', 'H', 'H', 'H',
    'H', 'I', 'I', 'I', 'I', 'J', 'J', 'J', 'J', 'K', 'K', 'K', 'K', 'L', 'L',
    'L', 'L', 'M', 'M', 'M', 'M', 'N', 'N', 'N', 'N', 'O', 'O', 'O', 'O', 'P',
    'P', 'P', 'P', 'Q', 'Q', 'Q', 'Q', 'R', 'R', 'R', 'R', 'S', 'S', 'S', 'S',
    'T', 'T', 'T', 'T', 'U', 'U', 'U', 'U', 'V', 'V', 'V', 'V', 'W', 'W', 'W',
    'W', 'X', 'X', 'X', 'X', 'Y', 'Y', 'Y', 'Y', 'Z', 'Z', 'Z', 'Z', '0', '0',
    '0', '0', '1', '1', '1', '1', '2', '2', '2', '2', '3', '3', '3', '3', '4',
    '4', '4', '4', '5', '5', '5', '5', '6', '6', '6', '7', '7', '7', '8', '8',
    '8', '9', '9', '9', '_', '_', '_', '.', '.', '.', '-', '-', '-', '~', '~',
    '~'};

const unsigned char kBase64[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

// Expects a randomly generated unsigned char, returns a valid char for a PKCE
// code verifier
void MapForVerifier(unsigned char &c) { c = kPkceChars[c]; }

// Generates a random vector of chars consisting of a-Z, 0-9, _.-~
// Returns an empty vector if length is not between 43 - 128
std::vector<unsigned char> GenPkceVerifier(std::size_t length) {
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

std::array<unsigned char, 32> Sha256(const std::vector<unsigned char> &data) {
  std::array<unsigned char, 32> cArr; // 32 is size of SHA256 output
  SHA256(data.data(), data.size(), cArr.data());
  return cArr;
}

// Based off RFC 4648 Section 4
std::vector<unsigned char>
Base64Encode(const std::vector<unsigned char> &data) {
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
