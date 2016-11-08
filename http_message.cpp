#include <assert.h>

#include "http_message.h"
#include "utils.h"
namespace vincy{

HttpMessage::HttpMessage() :
    headersMap_(),
    path_(),
    version_(),
    method_(),
    status_(),
    statusCode_(0)
{

}

std::string &HttpMessage::operator[](const std::string &key)
{
    return headersMap_[key];
}

void HttpMessage::addHeaders(const std::string &key, const std::string &value)
{
    headersMap_[key] = value;
}

void HttpMessage::setStatusLine(const std::string &version, int statusCode, const std::string &status)
{
    type_ = TYPE_HTTP_RESPONSE;
    version_ = version;
    statusCode_ = statusCode;
    status_ = status;
}

void HttpMessage::setRequestLine(int method, const std::__cxx11::string &path, const std::__cxx11::string &version)
{
    type_ = TYPE_HTTP_REQUEST;
    method_ = method;
    path_ = path;
    version_ = version;
}

void HttpMessage::setRequestLine(const std::string &method, const std::string &path, const std::string &version)
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

std::string HttpMessage::toString() const
{
    std::string line;
    std::string msg;
    if(type_ == TYPE_HTTP_REQUEST){
        line = int2string(method_) + " " + path_ + " " + version_ + "\r\n";
    }else{
        line = version_ + " " + int2string(statusCode_) + " " + status_ + "\r\n";
    }
    msg.append(line);

    for(const auto &w : headersMap_){
        line = w.first + ": " + w.second + "\r\n";
        msg.append(line);
    }

    msg.append("\r\n");
    return msg;
}

int HttpMessage::method() const
{
    return method_;
}

std::string HttpMessage::path() const
{
    return path_;
}

std::string HttpMessage::version() const
{
    return version_;
}

int HttpMessage::statusCode() const
{
    return statusCode_;
}

void HttpMessage::parse(const std::string &content, int type)
{
    type_ = type;
    assert(content.length() > 0);
    std::string line;
    std::stringstream ss(content);
    std::getline(ss, line, '\r');
    size_t pos;
    size_t left;
    if(type_ == TYPE_HTTP_REQUEST){
        //method
        pos = line.find(" ");
        assert(pos != std::string::npos);
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
        assert(pos != std::string::npos);
        path_ = line.substr(left, pos - left);
        //version
        left = pos + 1;
        pos = line.length();
        version_ = line.substr(left, pos - left);
    }else{
        //version
        pos = line.find(" ");
        assert(pos != std::string::npos);
        version_ = line.substr(0, pos);
        //status code
        left = pos + 1;
        pos = line.find(" ", left);
        assert(pos != std::string::npos);
        std::stringstream codess;
        codess << line.substr(left, pos);
        codess >> statusCode_;
    }
    //让getline用\r来当换行符,则下一行的开头会是'\n'.
    //因为设置以后他不具备换行作用了.
    headersMap_.clear();
    while(getline(ss, line, '\r')){
        //name
        pos = line.find(":");
        if(pos == std::string::npos){
            break;
        }
        std::string name = line.substr(1, pos - 1);
        //value
        left = pos + 2;
        pos = line.length() - 1;
        std::string value = line.substr(left, pos);
        trim(value);//去掉左右空格
        headersMap_[name] = value;
    }
}

HttpMessage::HeadersMap::iterator HttpMessage::find(const std::__cxx11::string &key)
{
    return headersMap_.find(key);
}

HttpMessage::HeadersMap::iterator HttpMessage::begin()
{
    return headersMap_.begin();
}

HttpMessage::HeadersMap::iterator HttpMessage::end()
{
    return headersMap_.end();
}

HttpMessage::HeadersMap::const_iterator HttpMessage::begin() const
{
    return headersMap_.cbegin();
}

HttpMessage::HeadersMap::const_iterator HttpMessage::end() const
{
    return headersMap_.cend();
}

HttpMessage::HeadersMap::const_iterator HttpMessage::cbegin() const
{
    return headersMap_.cbegin();
}

HttpMessage::HeadersMap::const_iterator HttpMessage::cend() const
{
    return headersMap_.cend();
}

//============================================================================
//HttpRequest
void HttpRequest::parse(const std::string &content)
{
    HttpMessage::parse(content, TYPE_HTTP_REQUEST);
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


}

