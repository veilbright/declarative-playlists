#include "nlohmann/json_fwd.hpp"
#include "rule_tree.h"
#include "spotify_session.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/parse.h"
#include <cstdlib>
#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <unistd.h>
#include <vector>

int main(int argc, char *argv[]) {
    // Create rule tree from first argument
    if (argc < 2) {
        std::cout << "USAGE: ./declists <dec path>\n";
        exit(1);
    }
    YAML::Node dec_yaml = YAML::LoadFile(argv[1]);
    BaseRuleNode dec = BaseRuleNode(dec_yaml);
    std::cout << "Generated Rule Tree:\n" << dec << "\n";

    // Sign the user into Spotify
    SpotifySession spotify_session = SpotifySession("7b6c48703e8040d68e090058e0273bbc", "http://localhost:8989");
    std::cout << "Use the following link to sign into your account:\n"
              << spotify_session.GetAuthUrl({"playlist-modify-public"}) << "\n\n";
    spotify_session.HostRedirectServer();
    spotify_session.SetUserId();

    std::vector<std::string> track_uris;

    for (RuleNode rule : dec.rules) {
        if (!rule.get_subject().track.empty()) {
            std::string search_term = rule.to_string();
            std::cout << "Searching for \"" << search_term << "\"...";
            nlohmann::json track_json = spotify_session.Search(rule.to_string(), SpotifySession::kTrack);
            if (!track_json["tracks"]["items"][0]["uri"].is_null()) {
                track_uris.push_back(track_json["tracks"]["items"][0]["uri"]);
                std::cout << "found\n";
            } else {
                exit(2);
            }
        }
    }
    std::string playlist_id = spotify_session.CreatePlaylist(dec.get_name(), dec.get_description());
    std::cout << "Created playlist: \"" << dec.get_name() << "\"\n";
    sleep(2);
    spotify_session.AddPlaylistTracks(playlist_id, track_uris);
    std::cout << track_uris.size() << " tracks added to playlist: \"" << dec.get_name() << "\"\n";
}
