#include "http_handler.h"
namespace vincy {
HttpHandler::HttpHandler()
{

}

HttpHandler::HandleFunc HttpHandler::getHandleFunc(const std::string &path)
{
    return handleFuncs_[path];
}

void HttpHandler::setHandleFunc(const std::string &path, const HttpHandler::HandleFunc &func)
{
    handleFuncs_[path] = func;
}

}
