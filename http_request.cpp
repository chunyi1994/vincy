#include <assert.h>

#include "http_request.h"
namespace vincy {

HttpRequest::HttpRequest() :
    path_(),
    version_(),
    method_(METHOD_TYPE_UNKNOWN),
    rawData_(),
    datasMap_(),
    cookiesMap_()
{}

void HttpRequest::parse(const std::string &content)
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
    //method
    pos = line.find(" ");
    if(pos == std::string::npos)
    {
        log("不合格的报文");
        return;
    }
    //assert(pos != std::string::npos);
    std::string method = line.substr(0, pos);
    if(method == "get" || method == "GET")
    {
        method_ = METHOD_TYPE_GET;
    }
    else if(method == "post" || method == "POST")
    {
        method_ = METHOD_TYPE_POST;
    }
    else if(method == "put" || method == "PUT")
    {
        method_ = METHOD_TYPE_PUT;
    }
    else{
        method_ = METHOD_TYPE_UNKNOWN;
    }
    //path
    left = pos + 1;
    pos = line.find(" ", left);
    if(pos == std::string::npos)
    {
        log("不合格的报文");
        return;
    }
    path_ = line.substr(left, pos - left);
    //version
    left = pos + 1;
    pos = line.length();
    version_ = line.substr(left, pos - left);

    HttpMessage::parse(content);

    if(method_ == METHOD_TYPE_POST)
    {
        size_t pos = content.find("\r\n\r\n");
        if(pos != std::string::npos)
        {
            std::string data = content.substr(pos + 4, content.length() - pos - 4);
            rawData_ = std::move(data);
        }
    }

    if(method_ == METHOD_TYPE_GET)
    {
        size_t pos = path_.find("?");
        if(pos != std::string::npos)
        {
            rawData_ = path_.substr(pos + 1, path_.length() - pos - 1);
            path_ = path_.substr(0, pos);
        }
    }
}

void HttpRequest::parseData()
{
    //parse post 或者get的data
    std::vector<std::string> vec = split(rawData_, "&");
    for(const auto& w : vec)
    {
        size_t pos = w.find("=");
        if(pos == std::string::npos)
        {
            continue;
        }
        std::string key = w.substr(0, pos);
        std::string value = w.substr(pos + 1, w.length() - pos - 1);
        datasMap_[key] = value;
    }
}

void HttpRequest::setRequestLine(int method, const std::string &path, const std::string &version)
{
    method_ = method;
    path_ = path;
    version_ = version;
}

void HttpRequest::setRequestLine(const std::string &method, const std::string &path, const std::string &version)
{
    int methodInt;
    if(method == "get" || method == "GET")
    {
        methodInt = METHOD_TYPE_GET;
    }
    else if(method == "post" || method == "POST")
    {
        methodInt = METHOD_TYPE_POST;
    }
    else if(method == "put" || method == "PUT")
    {
        methodInt = METHOD_TYPE_PUT;
    }
    else
    {
        methodInt = METHOD_TYPE_UNKNOWN;
    }
    setRequestLine(methodInt, path, version);
}

int HttpRequest::method() const
{
    return method_;
}

std::string HttpRequest::path() const
{
    return path_;
}

std::string HttpRequest::version() const
{
    return version_;
}

std::string HttpRequest::form(const std::string &key) const
{
    auto iter = datasMap_.find(key);
    if(iter != datasMap_.end())
    {
        return iter->second;
    }
    else
    {
        return "";
    }
}

std::string HttpRequest::cookie(const std::string &key) const
{
    auto iter = cookiesMap_.find(key);
    if(iter == cookiesMap_.end())
    {
        return "";
    }

    return iter->second;
}

void HttpRequest::setCookie(const std::string &key, const std::string &value)
{
    cookiesMap_[key] =  value;
}

void HttpRequest::setData(const std::string &key, const std::string &value)
{
    datasMap_[key] =  value;
}

std::string HttpRequest::toString() const
{
    std::string path = path_;
    if(method_ == METHOD_TYPE_GET && !rawData_.empty())
    {
        path = path_ + "?" + rawData_;
    }
    std::string line = int2string(method_) + " " + path + " " + version_ + "\r\n";
    std::string msg(line);

    for(const auto &w : headersMap_){
        line = w.first + ": " + w.second + "\r\n";
        msg.append(line);
    }
    return msg;
}

void HttpRequest::parseCookie()
{
    //parse cookies
    std::string cookie = headersMap_["Cookie"];
    if(!cookie.empty())
    {
        std::vector<std::string> cookiesVec = split(cookie, ";");
        for(const auto& cookieStr : cookiesVec)
        {
            size_t pos = cookieStr.find("=");
            if(pos == std::string::npos)
            {
                continue;
            }
            std::string key = cookieStr.substr(0, pos);
            std::string value = cookieStr.substr(pos + 1, cookieStr.length() - pos - 1);
            trim(key);
            trim(value);
            trimQuo(value);
            cookiesMap_[key] = value;
        }
    }
}

void HttpRequest::setRawData(std::string data)
{
    rawData_ = std::move(data);
}


const std::string &HttpRequest::rawData() const
{
    return rawData_;
}



}
