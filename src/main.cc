#include "spotify_session.h"
#include <cstdlib>
#include <iostream>
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
    SpotifySession spotify_session = SpotifySession("7b6c48703e8040d68e090058e0273bbc", "http://localhost:8989");
    std::cout << "Use the following link to sign into your account:\n\n" << spotify_session.GetAuthUrl() << std::endl;
    spotify_session.HostRedirectServer();
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
        std::cerr << "ERROR: Failed to convert \"" << argv[1] << "\" to a valid port number\n";
        OutputUsage(argv[0]);
        std::exit(EXIT_FAILURE);
    }
}
