#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H
#include <map>
#include <string>
#include <functional>
namespace vincy {

class HttpRequest;
class HttpResponse;

class HttpHandler
{
public:
    typedef std::function<std::string(HttpRequest&, HttpResponse&)> HandleFunc;
public:
    HttpHandler();
    void setHandleFunc(const std::string& path, const HandleFunc& func);
    HandleFunc getHandleFunc(const std::string& path);
private:
    std::map<std::string, HandleFunc> handleFuncs_;
};

}
#endif // HTTP_HANDLER_H
