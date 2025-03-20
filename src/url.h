#ifndef URL
#define URL

#include <map>
#include <string>

class Url {
  public:
    Url(const std::string &url);

    std::map<std::string, std::string> get_params() { return params; };
    void set_params(std::map<std::string, std::string> params) {
        this->params = params;
    };

    std::string EncodeParams(const std::string &str);
    void InsertParam(const std::string &key, const std::string &value);

    std::string to_string();
    std::string get_url_string();
    std::string get_params_string();
    std::map<std::string, std::string> params;

  private:
    std::string Encode(const std::string &str);
    std::string Decode(const std::string &str);
    std::string baseUrl;
};

#endif
