#include <assert.h>

#include "http_message.h"
#include "utils.h"
namespace vincy{

HttpMessage::HttpMessage() :
    headersMap_()
{

}

std::string &HttpMessage::operator[](const std::string &key)
{
    return headersMap_[key];
}

void HttpMessage::addHeader(const std::string &key, const std::string &value)
{
    headersMap_[key] = value;
}

std::string HttpMessage::toString() const
{
    return "";
}

void HttpMessage::parse(const std::string &content)
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

}

