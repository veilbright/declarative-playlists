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
    SpotifySession spotify_session = SpotifySession("7b6c48703e8040d68e090058e0273bbc", "http://localhost:8989");
    std::cout << "Use the following link to sign into your account:\n\n"
              << spotify_session.GetAuthUrl({"playlist-modify-public"}) << std::endl;
    spotify_session.HostRedirectServer();
    spotify_session.SetUserId();

    YAML::Node dec_yaml = YAML::LoadFile("decs/alternate.yaml");
    BaseRuleNode dec = BaseRuleNode(dec_yaml);

    spotify_session.CreatePlaylist(dec.get_name(), dec.get_description());
    std::vector<std::string> track_uris;

    for (RuleNode rule : dec.rules) {
        if (!rule.get_subject().song.empty()) {
            nlohmann::json song_json = spotify_session.Search(rule.get_subject().song, SpotifySession::kTrack);
            track_uris.push_back(song_json["tracks"]["items"][0]["uri"]);
        }
    }
    for (std::string uri : track_uris) {
        std::cout << uri << "\n";
    }
}
