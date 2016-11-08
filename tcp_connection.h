#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H
#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <functional>
#include <string>

namespace vincy {

using namespace boost::asio::ip;
using boost::asio::io_service;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::shared_ptr<TcpConnection> Pointer;
    typedef boost::array<char, 4096> ConnBuffer;
    typedef std::function<void(Pointer)> CloseCallback;
    typedef std::function<void(Pointer, std::size_t)> ReadCallback;
public:
    TcpConnection(io_service& io_service);
    ~TcpConnection();
    ConnBuffer& buf();
    void start();
    static Pointer create(boost::asio::io_service& io_service);
    tcp::socket& socket();

    void setCloseCallback(const CloseCallback& cb);
    void setReadCallback(const ReadCallback& cb);
    void send(const std::string& msg);

    bool connecting() const;

private:
    void handleWrite(const boost::system::error_code &ec, std::size_t bytes_transfered);
    void handleRead(const boost::system::error_code &ec, std::size_t bytes_transfered);
    void handleClose();

private:
    ConnBuffer buf_;
    tcp::socket socket_;
    bool isConnecting_;
    CloseCallback closeCallback_;
    ReadCallback readCallback_;
};

}
#endif // TCP_CONNECTION_H
