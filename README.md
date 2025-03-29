# Declarative Playlists

## Development Info
A project I'm working on to learn more about C++, APIs, and networking. Uses [Crow](https://github.com/CrowCpp/Crow) for the web server, [cpr](https://github.com/libcpr/cpr) for http requests, and a [JSON](https://github.com/nlohmann/json) and [YAML](https://github.com/jbeder/yaml-cpp) library to parse their respective formats.

Currently, the project can authenticate the user using Spotify, and parse a YAML file for a playlist rule tree.

When finished, the project will allow a user to create a YAML or JSON file describing a playlist using rules, which can then be added to their Spotify account.

## To Build
Run these commands to generate the build files and build the project. The output will be an executable called "DeclarativePlaylists" in the main directory.
```
cmake -B build
cmake --build build
```

## Example of Planned Usage
#### YAML Declared Playlist
```YAML
name: Alternate Playlist
description: Could be anything
add:
- artist: Banshee
  song: PERFECT ANGEL
- artist: bodyimage
  remove:
  - album: Active Lovers
  - song: I AM
- artist: Crywolf
  remove:
  - album: Skeletons
    add:
      - Silk
```
This file will create a playlist in Spotify with the supplied name and description. It then will add songs based on the rules given. The root rule must always be 'add', and each item in the add list will be an artist, an album, or a song.

If multiple specifiers are given for one item, the app will use the less specific qualifiers as a filter. For example, this:
```YAML
- song: PERFECT ANGEL
```
will look for any song called "PERFECT ANGEL", while this:
```YAML
- song: PERFECT ANGEL
  artist: Banshee
```
will only return songs by Banshee called "PERFECT ANGEL". The qualifiers can be in any order.

The other rule that can be used while declaring a playlist is 'remove'. This will remove songs added in parent rules. For example:
```YAML
- artist: bodyimage
  remove:
  - album: Active Lovers
```
will add all songs by bodyimage, and then remove the songs from "Active Lovers". Parent rules will also be used as qualifiers when searching for songs to remove, so the remove rule will search for albums and songs by bodyimage.

Rules of any depth will be supported. For example:
```YAML
- artist: Crywolf
  remove:
  - album: Skeletons
    add:
      - Silk
```
will add all songs by Crywolf, excluding those from "Skeletons" that aren't called "Silk".
