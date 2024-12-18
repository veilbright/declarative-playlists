#include <vector>

std::vector<unsigned char> GenPkceVerifier(std::size_t length);
std::array<unsigned char, 32> Sha256(const std::vector<unsigned char> &data);
std::vector<unsigned char> Base64Encode(const std::vector<unsigned char> &data);
