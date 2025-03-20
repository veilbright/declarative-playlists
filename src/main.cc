#include "crow/app.h"
#include "crow/http_request.h"
#include "spotify_api_requester.h"
#include "url.h"
#include <asio/impl/write.hpp>
#include <cstdlib>
#include <unistd.h>

// TODO:
// 1. Create server at requested port on localhost
// 2. When clients connect, authenticate them through Spotify
// 3. Once authenticated, request the YAML of their playlist
// 4. Search Spotify and create a list of URIs for songs to add
// 5. Create a playlist on their profile with the specified name
// 6. Disconnect from the client

void OutputUsage(std::string programName);
int HandleArgs(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    int port = HandleArgs(argc, argv);

    SpotifyApiRequester sar = SpotifyApiRequester(
        "7b6c48703e8040d68e090058e0273bbc", "http://localhost:3000");
    std::cout << "Use the following link to sign into your account: "
              << sar.GetAuthenticateUrl().to_string() << std::endl;

    // Tiny web server to host redirect URI and capture authCode parameter
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([&sar, &app](const crow::request &req) {
        sar.auth_code = req.url_params.get("code");
        if (sar.auth_code.empty()) {
            std::cout << "Authorization failed due to: "
                      << req.url_params.get("error");
        }
        app.stop();
        return "You can go back to the terminal now";
    });
    app.bindaddr("127.0.0.1").port(port).run();
    sar.GetAccessToken();
}

// Outputs the correct usage of the application
void OutputUsage(std::string programName) {
    std::cerr << "USAGE: " << programName << " <port-number>\n";
}

// Check for errors from the args and returns the port to run the server on
int HandleArgs(int argc, char *argv[]) {
    std::size_t *pos;

    if (argc != 2) {
        std::cerr << "ERROR: Incorrect number of arguments\n";
        OutputUsage(argv[0]);
        std::exit(EXIT_FAILURE);
    }
    try {
        return std::stoi(argv[1]);
    } catch (const std::exception e) {
        std::cerr << "ERROR: Failed to convert \"" << argv[1]
                  << "\" to a valid port number\n";
        OutputUsage(argv[0]);
        std::exit(EXIT_FAILURE);
    }
}
