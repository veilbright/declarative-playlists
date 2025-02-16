#include "spotify_api_requester.h"
#include "crypto_util.h"
#include <array>
#include <sstream>
#include <string>
#include <vector>

using std::ostringstream;
using std::string;
using std::vector;

// Returns the URL to the Spotify authenticator
// Uses the PKCE flow
Url SpotifyApiRequester::Authenticate() {
  const vector<unsigned char> kCodeVerifier = CryptoUtil::GenPkceVerifier(64);

  const std::array<unsigned char, 32> kHashArr =
      CryptoUtil::Sha256(kCodeVerifier);
  const vector<unsigned char> kCodeHash =
      vector<unsigned char>(kHashArr.begin(), kHashArr.end());

  const vector<unsigned char> kCodeChallengeVec =
      CryptoUtil::Base64Encode(kCodeHash);
  const string kCodeChallenge =
      string(kCodeChallengeVec.begin(), kCodeChallengeVec.end());

  Url authUrl = Url("https://accounts.spotify.com/authorize");
  authUrl.set_params({{"client_id", clientId},
                      {"response_type", "code"},
                      {"redirect_uri", authRedirectUri},
                      {"code_challenge_method", "S256"},
                      {"code_challenge", kCodeChallenge}});
  return authUrl;
}
