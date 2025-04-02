#ifndef DEC_EXCEPTION
#define DEC_EXCEPTION

#include <exception>
#include <string>

// TODO: Organize better and add 500 status errors

class HttpException : public std::exception {
  private:
    std::string message;

  public:
    HttpException(const char *message) : message(message) {
    }
    const char *what() const throw() {
        return message.c_str();
    }
};

class BadRequest : public HttpException {
  public:
    BadRequest(const char *message) : HttpException(message) {
    }
};

class Unauthorized : public HttpException {
  public:
    Unauthorized(const char *message) : HttpException(message) {
    }
};

class Forbidden : public HttpException {
  public:
    Forbidden(const char *message) : HttpException(message) {
    }
};

class NotFound : public HttpException {
  public:
    NotFound(const char *message) : HttpException(message) {
    }
};

class TooManyRequests : public HttpException {
  public:
    TooManyRequests(const char *message) : HttpException(message) {
    }
};

class InternalServerError : public HttpException {
  public:
    InternalServerError(const char *message) : HttpException(message) {
    }
};

#endif // !DEC_EXCEPTION
