#include <assert.h>
#include "http_response.h"
namespace vincy {

HttpResponse::HttpResponse() :
    cookies_(), version_(), status_(), statusCode_(0)
{

}

void HttpResponse::parse(const std::string &content)
{
    if(content.length() == 0)
    {
        log("不合格的报文");
        return;
    }
    std::string line;
    std::stringstream ss(content);
    std::getline(ss, line, '\r');
    size_t pos;
    size_t left;

    //version
    pos = line.find(" ");
    if(pos == std::string::npos)
    {
        log("不合格的报文");
        return;
    }
    version_ = line.substr(0, pos);
    //status code
    left = pos + 1;
    pos = line.find(" ", left);
    if(pos == std::string::npos)
    {
        log("不合格的报文");
        return;
    }
    std::stringstream codess;
    codess << line.substr(left, pos);
    codess >> statusCode_;

    HttpMessage::parse(content);

}

void HttpResponse::setStatusLine(const std::string &version, int statusCode, const std::string &status)
{
    version_ = version;
    statusCode_ = statusCode;
    status_ = status;
}

std::string HttpResponse::toString() const
{
    std::string line = version_ + " " + int2string(statusCode_) + " " + status_ + "\r\n";

    std::string ret(line);

    for(const auto &w : headersMap_){
        line = w.first + ": " + w.second + "\r\n";
        ret.append(line);
    }

    for(const auto& w : cookies_)
    {
        line = "Set-Cookie: " + w.toString() + "\r\n";
        ret.append(line);
    }
    return ret;
}

void HttpResponse::setCookie(std::string key, std::string value, std::string path, std::string domain, std::string expires)
{
    cookies_.push_back(Cookie(key,value, path ,domain, expires));
}

std::string HttpResponse::statusStr() const
{
    return status_;
}

std::string HttpResponse::version() const
{
    return version_;
}

int HttpResponse::statusCode() const
{
    return statusCode_;
}

}


