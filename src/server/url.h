#ifndef URL
#define URL

#include <map>
#include <string>

using std::map;
using std::string;

class Url {
public:
  Url(const string &url);

  map<string, string> get_params() { return params; };
  void set_params(map<string, string> params) { this->params = params; };

  string EncodeParams(const string &str);
  void InsertParam(const string &key, const string &value);

  string to_string();
  map<string, string> params;

private:
  string Encode(const string &str);
  string Decode(const string &str);
  string baseUrl;
};

#endif
