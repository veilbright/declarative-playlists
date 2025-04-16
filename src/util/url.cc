#include "url.h"
#include <cstddef>
#include <iterator>
#include <sstream>

// Constructs a URL object from the argument (made up of a base and the params)
Url::Url(const std::string &url) {
    std::ostringstream ssUrl;

    // pos will divide baseUrl and params
    size_t paramStart = url.find('?');

    // '?' not found, constructor is done after setting baseUrl
    if (paramStart == url.npos) {
        baseUrl = url;
        return;
    }
    // Set baseUrl
    baseUrl = url.substr(0, paramStart);

    // Parse params
    size_t nextPos = paramStart;
    while (nextPos != url.npos) {
        size_t equalPos = url.find('=', nextPos + 1);
        if (equalPos == url.npos) { // shouldn't happen, but we'll go along with it
            // Set key, ignore value
            InsertParam(Decode(url.substr(nextPos + 1)), "");
            break;
        }
        std::string key = Decode(url.substr(nextPos + 1, equalPos - (nextPos + 1)));
        std::string value;

        nextPos = url.find('&', equalPos);

        // value continues to the next param, or end of the std::string
        if (nextPos == url.npos) {
            value = Decode(url.substr(equalPos + 1));
        } else {
            value = Decode(url.substr(equalPos + 1, nextPos - (equalPos + 1)));
        }
        InsertParam(key, value);
    }
}

void Url::InsertParam(const std::string &key, const std::string &value) {
    parameters.insert({key, value});
};

std::string Url::to_string() const {
    // only return baseUrl if that's all there is
    if (parameters.empty()) {
        return get_url_string();
    }

    std::ostringstream url;
    url << get_url_string() << '?' << get_params_string();

    return url.str();
}

std::string Url::get_url_string() const {
    return baseUrl;
}

std::string Url::get_params_string() const {
    std::ostringstream params_string;
    for (std::map<std::string, std::string>::const_iterator it = parameters.begin(); it != parameters.end(); ++it) {
        params_string << Encode(it->first) << '=' << Encode(it->second);
        if (std::next(it) != parameters.end()) {
            params_string << '&';
        }
    }
    return params_string.str();
}

// Simple encoding, converts disallowed chars to %XX
std::string Url::Encode(const std::string &str) const {
    std::ostringstream ss;
    for (char c : str) {
        if (isalnum(c) || c == '_' || c == '~' || c == '.' || c == '-') {
            ss << c;
            continue;
        }
        ss << std::uppercase;
        ss << '%' << std::hex << int((unsigned char)c);
        ss << std::nouppercase;
    }
    return ss.str();
}

// Simple decoding, converts %XX to char
std::string Url::Decode(const std::string &str) const {
    std::ostringstream ss;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] != '%') {
            ss << str[i];
            continue;
        }
        if (i + 2 >= str.size()) { // ignore any extra % chars
            break;
        }
        // convert from hex to char
        int decimal = std::stoi(str.substr(i + 1, 2), nullptr, 16);
        if (decimal > 255 || decimal < 0) { // skip char if invalid ascii
            i += 2;
            continue;
        }
        ss << (unsigned char)decimal;
        i += 2;
    }
    return ss.str();
}
