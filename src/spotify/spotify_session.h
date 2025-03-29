#ifndef SPOTIFY_SESSION
#define SPOTIFY_SESSION

#include "cpr/parameters.h"
#include "nlohmann/json_fwd.hpp"
#include "url.h"
#include <string>

class SpotifySession {
  public:
    SpotifySession(std::string client_id, std::string redirect_uri) : client_id(client_id), redirect_uri(redirect_uri) {
    }
    const std::string GetAuthUrl();
    void HostRedirectServer(); // Host a basic web server to capture responses from Spotify

  private:
    void RequestAccessToken(); // set access_token to the value provided by Spotify
    const nlohmann::json RequestAPI(const std::string url_path, cpr::Parameters &parameters);

    const std::string kAPI_URL = "https://api.spotify.com/v1"; // base URL to access spotify API
    std::string auth_code;                                     // Auth code from the redirectUri after the user signs in
    std::string access_token;
    std::string code_verifier; // Used to get the access token
    std::string client_id;     // App ID from Spotify
    std::string redirect_uri;  // URI to process the authorization code in
};

#endif
