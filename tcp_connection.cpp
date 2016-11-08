#include <functional>
#include <string>

#include "tcp_connection.h"
#include "log.h"

namespace vincy {

TcpConnection::TcpConnection(io_service &io_service) :
    buf_(), socket_(io_service), isConnecting_(false)
{

}

TcpConnection::~TcpConnection()
{
    log_print("~TcpConnection");
}

TcpConnection::ConnBuffer &TcpConnection::buf()
{
    return buf_;
}

void TcpConnection::start()
{
    isConnecting_ = true;
    socket_.async_read_some(boost::asio::buffer(buf_), std::bind(
                                &TcpConnection::handleRead, this, std::placeholders::_1, std::placeholders::_2));
}

TcpConnection::Pointer TcpConnection::create(io_service &io_service)
{
    return std::make_shared<TcpConnection>(io_service);
}

tcp::socket &TcpConnection::socket()
{
    return socket_;
}

void TcpConnection::setCloseCallback(const TcpConnection::CloseCallback &cb)
{
    closeCallback_ = cb;
}

void TcpConnection::setReadCallback(const TcpConnection::ReadCallback &cb)
{
    readCallback_ = cb;
}

void TcpConnection::send(const std::string &msg)
{
    boost::asio::async_write(socket_, boost::asio::buffer(msg), std::bind(
                                 &TcpConnection::handleWrite, this, std::placeholders::_1, std::placeholders::_2));
}

bool TcpConnection::connecting() const
{
    return isConnecting_;
}

void TcpConnection::handleWrite(const boost::system::error_code &ec, std::size_t bytes_transfered)
{
}

void TcpConnection::handleRead(const boost::system::error_code &ec, std::size_t bytes_transfered)
{
    if(!ec)
    {
        //std::string recv(buf_.data(), bytes_transfered);
        //send(recv);
        if(readCallback_)
        {
            readCallback_(shared_from_this(), bytes_transfered);
        }
        socket_.async_read_some(boost::asio::buffer(buf_), std::bind(
                                    &TcpConnection::handleRead, this, std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        if(ec.value() == boost::asio::error::eof) // EOF
        {
            handleClose();
        }
    }

}

void TcpConnection::handleClose()
{
    isConnecting_ = false;
    if(closeCallback_){
        closeCallback_(shared_from_this());
    }
}

}

