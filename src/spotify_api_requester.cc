#include "spotify_api_requester.h"
#include "cpr/cpr.h"
#include "cpr/parameters.h"
#include "crypto_util.h"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <openssl/sha.h>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

// Returns the URL to the Spotify authenticator
// Uses the PKCE flow
Url SpotifyApiRequester::GetAuthenticateUrl() {
    // Generate and save the code verifier
    code_verifier = CryptoUtil::GenPkceVerifier(64);

    // Hash the PKCE code verifier using SHA256
    std::array<unsigned char, 32>
        code_verifier_sha; // 32 is size of SHA256 output
    SHA256(reinterpret_cast<const unsigned char *>(code_verifier.c_str()),
           code_verifier.length(), code_verifier_sha.data());

    // Base64 encode the hash
    std::vector<unsigned char> code_challenge_vec =
        CryptoUtil::Base64Encode(std::vector<unsigned char>(
            code_verifier_sha.begin(), code_verifier_sha.end()));
    std::ostringstream code_challenge_oss;

    // Convert Base64 vector to stringstream
    // Replace = + / because Spotify requires this
    for (int i = 0; i < code_challenge_vec.size(); ++i) {
        switch (code_challenge_vec[i]) {
        case '=':
            break;
        case '+':
            code_challenge_oss << '-';
            break;
        case '/':
            code_challenge_oss << '_';
            break;
        default:
            code_challenge_oss << code_challenge_vec[i];
        }
    }

    // Return the complete URL
    Url userAuthUrl = Url("https://accounts.spotify.com/authorize");
    userAuthUrl.set_params({{"client_id", client_id},
                            {"response_type", "code"},
                            {"redirect_uri", redirect_uri},
                            {"code_challenge_method", "S256"},
                            {"code_challenge", code_challenge_oss.str()}});
    return userAuthUrl;
}

std::string SpotifyApiRequester::GetAccessToken() {
    std::cout << "VERIFIER: '" << code_verifier << "'" << std::endl;
    cpr::Response token_response =
        cpr::Post(cpr::Url("https://accounts.spotify.com/api/token"),
                  cpr::Parameters{{"grant_type", "authorization_code"},
                                  {"code", auth_code},
                                  {"redirect_uri", redirect_uri},
                                  {"client_id", client_id},
                                  {"code_verifier", code_verifier}});
    std::cout << token_response.status_code << std::endl;
    std::cout << token_response.text << std::endl;
    return "";
}
