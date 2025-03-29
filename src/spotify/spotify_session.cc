#include "spotify_session.h"
#include "cpr/api.h"
#include "cpr/auth.h"
#include "cpr/parameters.h"
#include "cpr/response.h"
#include "crow/app.h"
#include "crow/http_request.h"
#include "crypto_util.h"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "url.h"
#include <array>
#include <crow/json.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <openssl/sha.h>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

const nlohmann::json SpotifySession::RequestAPI(const std::string url_path, cpr::Parameters &parameters) {
    cpr::Response response = cpr::Get(cpr::Url(kAPI_URL + url_path), cpr::Bearer(access_token), parameters);
    std::cout << std::endl << response.text << std::endl << std::endl;
    return nlohmann::json::parse(response.text);
}

// Returns the URL to the Spotify authenticator
// Uses the PKCE flow
const std::string SpotifySession::GetAuthUrl() {
    // Generate and save the code verifier
    code_verifier = CryptoUtil::GenPkceVerifier(64);

    // Hash the PKCE code verifier using SHA256
    std::array<unsigned char, 32> code_verifier_sha; // 32 is size of SHA256 output
    SHA256(reinterpret_cast<const unsigned char *>(code_verifier.c_str()), code_verifier.length(),
           code_verifier_sha.data());

    // Base64 encode the hash
    std::vector<unsigned char> code_challenge_vec =
        CryptoUtil::Base64Encode(std::vector<unsigned char>(code_verifier_sha.begin(), code_verifier_sha.end()));
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
    return userAuthUrl.to_string();
}

// Tiny web server to host redirect URI and capture authCode parameter
void SpotifySession::HostRedirectServer() {
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([&auth_code = this->auth_code, &app](const crow::request &req) {
        auth_code = req.url_params.get("code");
        if (auth_code.empty()) {
            // TODO: LOGGING
            std::cout << "Authorization failed due to: " << req.url_params.get("error");
        }
        app.stop();
        return "You can go back to the terminal now";
    });
    app.loglevel(crow::LogLevel::Warning);
    app.bindaddr("127.0.0.1").port(8989).run();
    RequestAccessToken();
}

void SpotifySession::RequestAccessToken() {
    cpr::Response token_response = cpr::Post(cpr::Url("https://accounts.spotify.com/api/token"),
                                             cpr::Parameters{{"grant_type", "authorization_code"},
                                                             {"code", auth_code},
                                                             {"redirect_uri", redirect_uri},
                                                             {"client_id", client_id},
                                                             {"code_verifier", code_verifier}});
    nlohmann::json token_json = nlohmann::json::parse(token_response.text);
    if (token_response.status_code != 200) {
        // TODO: better error msg and log (use json from Spotify's response)
        throw std::runtime_error("Received invalid response from Spotify");
    }
    for (nlohmann::json::iterator it = token_json.begin(); it != token_json.end(); ++it) {
        if (it.key() == "access_token") {
            access_token = std::string(*it);
            return;
        }
    }
    // TODO: LOG
    throw std::runtime_error("Unable to acquire access_token from Spotify");
}
