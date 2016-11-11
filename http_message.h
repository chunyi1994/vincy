#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H
#include <string>
#include <map>
#include <vector>
#include "utils.h"
#include "cookie.h"
namespace vincy{

const int TYPE_HTTP_RESPONSE = 0;
const int TYPE_HTTP_REQUEST = 1;


class HttpMessage
{
    typedef std::map<std::string, std::string> HeadersMap;

public:
    HttpMessage();
    std::string& operator[](const std::string& key);

    void addHeader(const std::string& key,const  std::string& value);

    std::string toString() const;    //这个操作比较费时,每次调用都会重新解析一次

    void parse(const std::string &content);

    HeadersMap::iterator find(const std::string& key);
    HeadersMap::iterator begin();
    HeadersMap::iterator end();
    HeadersMap::const_iterator begin() const;
    HeadersMap::const_iterator end() const;
    HeadersMap::const_iterator cbegin() const;
    HeadersMap::const_iterator cend() const;

protected:
    HeadersMap headersMap_;   
};




const int METHOD_TYPE_GET = 0;
const int METHOD_TYPE_POST = 1;
const int METHOD_TYPE_PUT = 2;
const int METHOD_TYPE_UNKNOWN = 3;

}
/*
        msg["Host"] = "cn.bing.com";          //msg.addHeaders("Host","cn.bing.com");
        msg["Connection"] = "keep-alive";  //msg.addHeaders("Connection","keep-alive");
        std::cout << msg.toString() << std::endl;

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
