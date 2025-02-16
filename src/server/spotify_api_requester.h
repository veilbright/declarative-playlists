#ifndef SPOTIFY_API_REQUESTER
#define SPOTIFY_API_REQUESTER

#include "url.h"
#include <string>

using std::string;

class SpotifyApiRequester {
public:
  SpotifyApiRequester(string clientId, string authRedirectUri)
      : clientId(clientId), authRedirectUri(authRedirectUri) {}
  Url Authenticate();

private:
  string clientId;        // App ID from Spotify
  string authRedirectUri; // URI to process the authorization code in
};

#endif
