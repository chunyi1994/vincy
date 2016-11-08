#include <functional>
#include <utility>

#include "http_server.h"
#include "log.h"
#include "utils.h"
#include "http_utils.h"

namespace vincy {

HttpServer::HttpServer(io_service &ioservice, size_t port) :
    acceptor_(ioservice, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    conns_(),
    handler_()
{
    startAccept();
}

void HttpServer::handleFunc(const std::string &path, const HttpHandler::HandleFunc &func)
{
    handler_.setHandleFunc(path, func);
}

void HttpServer::startAccept()
{
    TcpConnection::Pointer conn = TcpConnection::create(acceptor_.get_io_service());
    acceptor_.async_accept(conn->socket(), std::bind(&HttpServer::handleAccept, this, conn, std::placeholders::_1));
}

void HttpServer::handleAccept(TcpConnection::Pointer conn,  const boost::system::error_code &ec)
{
    if(!ec)
    {
        conns_.insert(conn);
        conn->setCloseCallback(std::bind(&HttpServer::handleClose, this, std::placeholders::_1));
        conn->setReadCallback(std::bind(&HttpServer::handleRead, this,  std::placeholders::_1,  std::placeholders::_2));
        conn->start();
    }
    startAccept();
}

void HttpServer::handleClose(TcpConnection::Pointer conn)
{
    conns_.erase(conn);
}

void sendFile(TcpConnection::Pointer conn, int fd){
    char buf[1024];
    int n = 0;
    while(1){
        n = read(fd, buf, sizeof(buf));
        if(n <= 0)break;
        std::string msg(buf, n);
        conn->send(msg);
    }
}

void send404(TcpConnection::Pointer conn)
{
    HttpMessage response;
    response.setStatusLine("HTTP/1.1", 404, "NOT FOUND");
    std::string sendMsg("no page!");
    response["Content-Length"]  = int2string(sendMsg.length());
    conn->send(response.toString());
    conn->send(sendMsg);
}

void HttpServer::handleRead(TcpConnection::Pointer conn, std::size_t bytes_transfered)
{
    std::string recv(conn->buf().data(), bytes_transfered);
    HttpRequest request;
    request.parse(recv);
    std::string path = request.path();

    HttpResponse response;
    response["Connection"] = "keep-alive";
    //如果是访问静态文件,则读取文件,返回
    if(beginWith(path, "/static"))
    {
        path = path.substr(1, path.length() - 1);

        int fd = open(path.c_str(), O_RDONLY);
        if(fd == -1){
            send404(conn);
            return;
        }
        response.setStatusLine("HTTP/1.1", 200, "OK");
        response["Content-Type"] = contentType(path);
        response["Content-Length"] = int2string(getFileSize(path.c_str()));
        conn->send(response.toString());
        sendFile(conn, fd);
        close(fd);
        return;
    }
    //如果访问的是记录在案的路由地址,则调用function
    HttpHandler::HandleFunc handleFunction = handler_.getHandleFunc(path);
    if(handleFunction)
    {
        response.setStatusLine("HTTP/1.1", 200, "OK");
        std::string sendMsg = std::move(handleFunction(request));
        response["Content-Length"] = int2string(sendMsg.length());
        conn->send(response.toString());
        conn->send(sendMsg);
    }
    else
    {
        send404(conn);
    }
}

}
