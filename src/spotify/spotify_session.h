#ifndef SPOTIFY_SESSION
#define SPOTIFY_SESSION

#include "cpr/parameters.h"
#include "http_method.h"
#include "nlohmann/json_fwd.hpp"
#include <exception>
#include <string>

class SpotifySession {
  public:
    SpotifySession(std::string client_id, std::string redirect_uri) : client_id(client_id), redirect_uri(redirect_uri) {
    }

    enum Type {
        kTrack = 0,
        kAlbum = 1,
        kArtist = 2,
    };
    std::vector<std::string> kTypeString = {"track", "album", "artist"};

    // Returns the URL string to a web page the user can visit to authenticate the session
    std::string GetAuthUrl(std::vector<std::string> scopes);

    // Host a basic web server to capture responses from Spotify
    void HostRedirectServer();

    nlohmann::json GetUser(); // Returns the current user from Spotify
    std::string CreatePlaylist(const std::string &name, const std::string &description, const bool is_public = true,
                               const bool collaborative = false) const;
    std::string AddPlaylistTracks(const std::string &playlist_id, const std::vector<std::string> &track_uris) const;
    nlohmann::json Search(const std::string &query, const Type type, const int limit = 1, const int offset = 0) const;
    void SetUserId(); // Sets the Session's User ID

  private:
    void RequestAccessToken(); // set access_token to the value provided by Spotify
    nlohmann::json RequestAPI(const std::string &url_path, const HttpMethod method, const cpr::Parameters &parameters,
                              const nlohmann::json &body) const;

    const std::string kAPI_URL = "https://api.spotify.com/v1"; // base URL to access spotify API
    std::string auth_code;                                     // Auth code from the redirectUri after the user signs in
    std::string access_token;
    std::string code_verifier; // Used to get the access token
    std::string client_id;     // App ID from Spotify
    std::string redirect_uri;  // URI to process the authorization code in
    std::string user_id;       // User ID used in certain requests
};

#endif
