#ifndef SPOTIFY_API_REQUESTER
#define SPOTIFY_API_REQUESTER

#include "url.h"
#include <string>

class SpotifyApiRequester {
  public:
    SpotifyApiRequester(std::string clientId, std::string authRedirectUri)
        : client_id(clientId), redirect_uri(authRedirectUri) {}
    std::string
        auth_code; // Auth code from the redirectUri after the user signs in
    Url GetAuthenticateUrl();
    std::string GetAccessToken();

  private:
    std::string code_verifier; // Used to get the access token
    std::string client_id;     // App ID from Spotify
    std::string redirect_uri;  // URI to process the authorization code in
};

#endif
