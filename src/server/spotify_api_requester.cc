#include "spotify_api_requester.h"
#include "spotify_authenticator.h"
#include <array>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::map;
using std::ostringstream;
using std::string;
using std::vector;

// Returns the URL to the Spotify authenticator
// Uses the PKCE flow
string SpotifyApiRequester::Authenticate() {
  const vector<unsigned char> kCodeVerifier = GenPkceVerifier(64);

  const std::array<unsigned char, 32> kHashArr = Sha256(kCodeVerifier);
  const vector<unsigned char> kCodeHash =
      vector<unsigned char>(kHashArr.begin(), kHashArr.end());

  const vector<unsigned char> kCodeChallengeVec = Base64Encode(kCodeHash);
  const string kCodeChallenge =
      string(kCodeChallengeVec.begin(), kCodeChallengeVec.end());

  return "";
  /*return BuildUrl("https://accounts.spotify.com/authorize",*/
  /*{{"client_id", clientId},*/
  /* {"response_type", "code"},*/
  /* {"redirect_uri", authRedirectUri},*/
  /* {"code_challenge_method", "S256"},*/
  /* {"code_challenge", kCodeChallenge}});*/
}
