#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H
#include "http_message.h"
namespace vincy {


class HttpResponse : public HttpMessage
{
public:
    HttpResponse();
    void parse(const std::string &content);
    void setStatusLine(const std::string& version, int statusCode,const std::string& status);
    void setCookie(std::string key, std::string value, std::string path = "",std::string domain = "", std::string expires = "");

    std::string statusStr() const;
    std::string version() const;
    int statusCode() const;
    std::string toString() const;

private:
    std::vector<Cookie> cookies_;
    std::string version_;
    std::string status_;
    int statusCode_;
};

}

#endif // HTTP_RESPONSE_H
