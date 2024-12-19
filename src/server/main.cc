#include "crow/app.h"
#include "spotify_api_requester.h"
#include "url.h"
#include <cstdlib>

// TODO:
// 1. Create server at requested port on localhost
// 2. When clients connect, authenticate them through Spotify
// 3. Once authenticated, request the JSON of their playlist
// 4. Search Spotify and create a list of URIs for songs to add
// 5. Create a playlist on their profile with the specified name
// 6. Disconnect from the client

void OutputUsage(std::string programName);
int HandleArgs(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  Url url = Url("https://api.spotify.com/v1/"
                "search?q=remaster%2520track%3ADoxy%2520artist%3AMiles%"
                "2520Davis&type=album");
  std::cout << url.to_string() << std::endl;
  /*int port = HandleArgs(argc, argv);*/
  /**/
  /*crow::SimpleApp app;*/
  /*CROW_ROUTE(app, "/")([]() { return "Hello World"; });*/
  /*app.bindaddr("127.0.0.1").port(port).run();*/
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
