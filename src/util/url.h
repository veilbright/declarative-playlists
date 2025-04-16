#ifndef URL
#define URL

#include <map>
#include <string>

typedef std::map<std::string, std::string> parameter_map;

class Url {
  public:
    Url(const std::string &url);

    std::map<std::string, std::string> get_params() const {
        return parameters;
    };
    void set_params(std::map<std::string, std::string> parameters) {
        this->parameters = parameters;
    };

    void InsertParam(const std::string &key, const std::string &value);

    std::string to_string() const;
    std::string get_url_string() const;
    std::string get_params_string() const;
    parameter_map parameters;

  private:
    std::string Encode(const std::string &str) const;
    std::string Decode(const std::string &str) const;
    std::string baseUrl;
};

#endif
