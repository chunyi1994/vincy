# vincy
vincy是一个c++ web框架. python-flask和go语言的web框架都非常简洁大方,所以我开发了一个相同用法的框架

这个框架使用非常简单, main.cpp里面提供了一个例子.

这个框架从tcp层开始接管,底层使用boost.asio,

也就是说，底层使用的是我自己开发的小型HTTP服务器， 所以不可以和nginx同时使用,也不是基于cgi的.

这个框架自带一个模板引擎,用法和jinja应该是差不多的.

所有的静态文件存在 /static 里,所有的html模板文件存在 /templates 里.

用法:

    在main函数中中,
    
    boost::asio::io_service ioservice;
 
    vincy::HttpServer server(ioservice, 80);

    server.handleFunc("/", handleIndex);
    
    这样就完成了/目录的注册, 接下来编写handleIndex函数
    
    std::string handleIndex(HttpRequest&, HttpResponse&)
    {
        return "hello world";
    }
    
    然后,访问127.0.0.1, 即可看到 hello world
    

模板引擎的语法如下:
{% for item in titles %} <a href="{{ item.url }}"> {{ item.title }}</a> {% endfor %}
    
我的名字是{{ myname }}
    
我的密码是{{ mypassword }}
