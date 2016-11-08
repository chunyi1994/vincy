#include <iostream>
#include <string>
#include "http_server.h"
#include "simple_template.h"
using namespace std;
using namespace vincy;

std::string handleIndex(HttpRequest& r)
{
    cout<<r.method()<<endl;
    //输出所有的header
    for(const auto& header : r){
        cout<<header.first<<" : "<<header.second<<endl;
    }

    //post或者get的原始data,一般是 user=user&password=122345这格式
    cout<<"data:" <<r.rawData()<<endl;

    //需要手动parseData,然后就可以通过form得到数据
    r.parseData();
    cout<<r.form("username")<<endl;
    return "hello world";
}

std::string handleHello(HttpRequest& r)
{
    SimpleTemplate t("index2.html");
    return t.toString();
}

std::string handleHey(HttpRequest& r)
{
    //hey.html的内容是:
    //<h1>我的名字是{{ myname }}</h1>
    //<h1>我的密码是{{ mypassword }}</h1>
    SimpleTemplate t("hey.html");
    t.setValue("myname", "gaochunyi");
    t.setValue("mypassword", "abc123456");
    return t.toString();
}

std::string handleFor(HttpRequest& r)
{
    //for.html的内容是:
    // {% for item in titles %} <a href="{{ item.url }}"> {{ item.title }}</a> {% endfor %}
    SimpleTemplate t("for.html");
    ForLoopVector vec;   //其实就是vector<ForItemsMap>

    ForItemMap map1;  //其实就是map<string, string>
    map1["title"] = "百度网";
    map1["url"] = "http://www.baidu.com";
    vec.push_back(map1);

    ForItemMap map2;
    map2["title"] = "腾讯网";
    map2["url"] = "http://www.qq.com";
    vec.push_back(map2);

    t.setForLoopValues("titles", vec);
    return t.toString();
}

int main() {
    boost::asio::io_service ioservice;
    vincy::HttpServer server(ioservice, 23333);

    server.handleFunc("/", handleIndex);
    server.handleFunc("/hello", handleHello);
    server.handleFunc("/hey", handleHey);
    server.handleFunc("/for", handleFor);

    ioservice.run();
    cout<<"good bye"<<endl;
    return 0;
}


//     boost::asio::io_service io;
//     boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
//     t.wait();
//     std::cout << "Hello, world!\n";

