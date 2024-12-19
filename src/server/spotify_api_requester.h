#include <string>

using std::string;

class SpotifyApiRequester {
public:
  SpotifyApiRequester(string clientId, string authRedirectUri)
      : clientId(clientId), authRedirectUri(authRedirectUri) {}
  string Authenticate();

private:
  string clientId;        // App ID from Spotify
  string authRedirectUri; // URI to process the authorization code in
};
