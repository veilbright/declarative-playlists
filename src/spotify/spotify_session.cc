#include "spotify_session.h"
#include "cpr/api.h"
#include "cpr/cprtypes.h"
#include "cpr/parameters.h"
#include "cpr/response.h"
#include "crow/app.h"
#include "crow/http_request.h"
#include "crypto_util.h"
#include "exception.h"
#include "http_method.h"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "util/url.h"
#include <array>
#include <crow/json.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <openssl/sha.h>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

nlohmann::json SpotifySession::Search(const std::string &query, const Type type, const int limit,
                                      const int offset) const {
    return RequestAPI("/search", kGet,
                      {{"q", query},
                       {"type", kTypeString[type]},
                       {"limit", std::to_string(limit)},
                       {"offset", std::to_string(offset)}},
                      {});
}

void SpotifySession::SetUserId() {
    try {
        nlohmann::json user_json = GetUser();
        user_id = user_json["id"];
    } catch (const HttpException &e) {
        std::cout << "Unable to set user_id: " << e.what() << "\n"; // TODO: Log
    }
}

nlohmann::json SpotifySession::GetUser() {
    return RequestAPI("/me", kGet, {}, {});
}

std::string SpotifySession::CreatePlaylist(const std::string &name, const std::string &description,
                                           const bool is_public, const bool collaborative) const {
    std::string path = "/users/" + user_id + "/playlists";
    return RequestAPI(
        path, kPost, {},
        {{"name", name}, {"description", description}, {"public", is_public}, {"collaborative", collaborative}})["id"];
}

std::string SpotifySession::AddPlaylistTracks(const std::string &playlist_id,
                                              const std::vector<std::string> &track_uris) const {
    std::string path = "/playlists/" + playlist_id + "/tracks";
    return RequestAPI(path, kPost, {}, {{"uris", nlohmann::json(track_uris)}})["snapshot_id"];
}

nlohmann::json SpotifySession::RequestAPI(const std::string &url_path, const HttpMethod method,
                                          const cpr::Parameters &parameters, const nlohmann::json &body) const {
    cpr::Response response;
    cpr::Url url = kAPI_URL + url_path;
    switch (method) {
    case kGet:
        response = cpr::Get(url, cpr::Bearer(access_token), parameters);
        break;
    case kPost:
        response = cpr::Post(url, cpr::Bearer(access_token), cpr::Header({{"Content-Type", "application/json"}}),
                             cpr::Body(body.dump()));
        break;
    case KPut:
        // TODO: Put request
        break;
    }
    nlohmann::json json = nlohmann::json::parse(response.text);
    // TODO: Exceptions cause the program to exit, regardless of catch statements
    // It probably has something to do with CMake or GCC options
    if (response.status_code != 200 && response.status_code != 201) {
        std::string message =
            "Request: " + url.str() + " Error: " + json["error"]["message"].template get<std::string>().c_str();
        switch (response.status_code) {
        case 400:
            throw BadRequest(message.c_str());
        case 401:
            throw Unauthorized(message.c_str());
        case 403:
            throw Forbidden(message.c_str());
        case 404:
            throw NotFound(message.c_str());
        case 429:
            throw TooManyRequests(message.c_str());
        }
    }
    return json;
}

// Returns the URL to the Spotify authenticator
// Uses the PKCE flow
std::string SpotifySession::GetAuthUrl(std::vector<std::string> scopes) {
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

    std::ostringstream scope_stream;
    for (std::string &scope : scopes) {
        scope_stream << scope << " ";
    }

    // Return the complete URL
    Url userAuthUrl = Url("https://accounts.spotify.com/authorize");
    userAuthUrl.set_params({{"client_id", client_id},
                            {"response_type", "code"},
                            {"redirect_uri", redirect_uri},
                            {"code_challenge_method", "S256"},
                            {"code_challenge", code_challenge_oss.str()},
                            {"scope", scope_stream.str()}});
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
