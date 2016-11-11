#include <iostream>
#include <string>
#include "http_server.h"
#include "simple_template.h"
using namespace std;
using namespace vincy;

std::string handleHelloWorld(HttpRequest&, HttpResponse&)
{
    return "hello world";
}

std::string handleIndex(HttpRequest& request,HttpResponse& response)
{
    if(request.method() == METHOD_TYPE_GET)
    {
        //输出请求报文第一行
        cout<<request.method()<<" " << request.path()<<" "<<request.version()<<endl;

        //输出所有的header
        for(const auto& header : request){
            cout<<header.first<<" : "<<header.second<<endl;
        }

        //post或者get的原始data,一般是 user=user&password=122345这格式
        cout<<"rawData:" <<request.rawData()<<endl;

        //需要手动parseData,然后就可以通过form得到数据
        request.parseData();
        //获得get或者post的数据（无论是get还是post统一用form函数获取）
        cout<<"username:"<<request.form("username")<<endl;

        //获得cookie
        cout<<"user.cookie:"<<request.cookie("user")<<endl;
        cout<<"time.cookie:"<<request.cookie("time")<<endl;

        //设置cookies
        response.setCookie("user", "Vincy");
        response.setCookie("time", "morninge");

        //获取以及设置session
        Session& sessions = gSessionManager.getSessions(request);
        if(sessions["name"] == "hello"){
            cout<<"yes!"<<endl;
        }else{
            cout<<"no"<<endl;
        }

        sessions["name"] = "hello";

        //为response添加header键值对
        response["headerKey"] = "headerValue";  //等价于response.addHeader("key", "value");


        return "hello world";
    }
    else if(request.method() == METHOD_TYPE_POST)
    {
        cout<<request.rawData()<<endl;
        request.parseData();
        cout<<"#"<<request.form("word")<<"#"<<endl;
        return "post";
    }
    else{
        return "other";
    }
}

std::string handleHello(HttpRequest& r,HttpResponse&)
{
    //模板引擎
    SimpleTemplate t("post.html");
    return t.toString();
}

std::string handleHey(HttpRequest& r, HttpResponse&)
{
    //模板引擎
    //hey.html的内容是:
    //<h1>我的名字是{{ myname }}</h1>
    //<h1>我的密码是{{ mypassword }}</h1>
    SimpleTemplate t("hey.html");
    t.setValue("myname", "gaochunyi");
    t.setValue("mypassword", "abc123456");
    return t.toString();
}

std::string handleFor(HttpRequest& r, HttpResponse&)
{
    //模板引擎
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

std::string handleRedirect(HttpRequest& r, HttpResponse&)
{
    //重定向
    return redirect("/");
}



int main() {

    boost::asio::io_service ioservice;
    vincy::HttpServer server(ioservice, 23333);

    server.handleFunc("helloworld", handleHelloWorld);
    server.handleFunc("/", handleIndex);
    server.handleFunc("/hello", handleHello);
    server.handleFunc("/hey", handleHey);
    server.handleFunc("/for", handleFor);
    server.handleFunc("/redirect", handleRedirect);

    ioservice.run();
    cout<<"good bye"<<endl;
    return 0;
}


//     boost::asio::io_service io;
//     boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
//     t.wait();
//     std::cout << "Hello, world!\n";

