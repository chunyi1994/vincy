#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H
#include <string>
#include <map>

#include "utils.h"
namespace vincy{

const int TYPE_HTTP_RESPONSE = 0;
const int TYPE_HTTP_REQUEST = 1;


class HttpMessage
{
    typedef std::map<std::string, std::string> HeadersMap;

public:
    HttpMessage();
    std::string& operator[](const std::string& key);

    void addHeaders(const std::string& key,const  std::string& value);
    void setStatusLine(const std::string& version, int statusCode,const std::string& status);
    void setRequestLine(int method, const std::string& path, const std::string& version);
    void setRequestLine(const std::string& method, const std::string& path, const std::string& version);
    std::string toString() const;    //这个操作比较费时,每次调用都会重新解析一次
    int method() const;
    std::string path() const;
    std::string version() const;
    int statusCode() const;
    void parse(const std::string &content, int type);

    HeadersMap::iterator find(const std::string& key);
    HeadersMap::iterator begin();
    HeadersMap::iterator end();
    HeadersMap::const_iterator begin() const;
    HeadersMap::const_iterator end() const;
    HeadersMap::const_iterator cbegin() const;
    HeadersMap::const_iterator cend() const;

protected:
    HeadersMap headersMap_;
    int type_;
    std::string path_;
    std::string version_;
    int method_;
    std::string status_;
    int statusCode_;
};

class HttpRequest : public HttpMessage
{
public:
    void parse(const std::string &content);
    void parseData();
    std::string form(const std::string &key) const;

    void setData(std::string data) {   rawData_ = std::move(data); }

    const std::string& rawData() const {    return rawData_;   }

private:
    void setStatusLine(const std::string& version, int statusCode,const std::string& status) = delete;
    int statusCode() const = delete;

private:
    std::string rawData_;
    std::map<std::string, std::string> datasMap_;
};


class HttpResponse : public HttpMessage
{

public:
    void parse(const std::string &content)
    {
        HttpMessage::parse(content, TYPE_HTTP_RESPONSE);
    }

private:
    int method() const = delete;
    std::string path() const = delete;
    void setRequestLine(const std::string& method, const std::string& path, const std::string& version) = delete;
    void setRequestLine(int method, const std::string& path, const std::string& version) = delete;

};

const int METHOD_TYPE_GET = 0;
const int METHOD_TYPE_POST = 1;
const int METHOD_TYPE_PUT = 2;
const int METHOD_TYPE_UNKNOWN = 3;

}

/*
        用来生成一个http msg报文
        HttpMessage msg;
        msg.setRequestLine("GET", "/", "HTTP/1.1");
        msg["Host"] = "cn.bing.com";          //msg.addHeaders("Host","cn.bing.com");
        msg["Connection"] = "keep-alive";  //msg.addHeaders("Connection","keep-alive");
        std::cout << msg.toString() << std::endl;
*/

/*
        用来解析一个http msg报文
        HttpMessage msg;
        string msgStr = "GET / HTTP/1.1\r\nHost: cn.bing.com\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n";
        msg.parse(msgStr, TYPE_HTTP_REQUEST);
        std::cout<< msg["Host"] << std::endl;

        for(auto w : msg){
            cout<<w.first<<endl;
            cout<<w.second<<endl;
        }

        for(auto iter = msg.begin();
                iter !+ msg.end();
                iter++){
            cout<<iter->first<<endl;
            cout<<iter->second<<endl;
        }
*/



#endif // HTTPMESSAGE_H
