#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "http_message.h"

namespace vincy {


class HttpRequest : public HttpMessage
{
    friend class HttpServer;
public:
    HttpRequest();

    void parse(const std::string &content);
    void parseData();  //框架不会主动 这个，需要用户自行调用

    void setRawData(std::string data);
    void setRequestLine(int method, const std::string& path, const std::string& version);
    void setRequestLine(const std::string& method, const std::string& path, const std::string& version);
    void setCookie(const std::string&key, const std::string& value);
    void setData(const std::string&key, const std::string& value);

    int method() const;
    std::string path() const;
    std::string version() const;
    std::string form(const std::string &key) const;
    std::string cookie(const std::string& key) const;


    std::string toString() const;
    const std::string& rawData() const;

private:
    void parseCookie();

private:
    std::string path_;
    std::string version_;
    int method_;
    std::string rawData_;
    std::map<std::string, std::string> datasMap_;
    std::map<std::string, std::string> cookiesMap_;
};



}

/*
        用来解析一个http msg报文
        HttpRequest msg;
        string msgStr = "GET / HTTP/1.1\r\nHost: cn.bing.com\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n";
        msg.parse(msgStr);

*/

#endif // HTTP_REQUEST_H
