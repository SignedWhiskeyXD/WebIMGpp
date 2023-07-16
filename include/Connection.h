//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_CONNECTION_H
#define WEBIMG_CONNECTION_H

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include "RequestParser.h"
#include "HTTPResponse.h"
#include "HTTPRequest.h"

class ConnectionPool;
class ThreadPool;

class Connection : public std::enable_shared_from_this<Connection>{
public:
    friend class RequestParser;

    explicit Connection(boost::asio::ip::tcp::socket sock, ConnectionPool& pool, ThreadPool& threadPool):
        socket(std::move(sock)), poolHandler(pool), queueHandler(threadPool) { }

    Connection() = delete;

    Connection(const Connection&) = delete;

    Connection& operator=(const Connection&) = delete;

    void info(){
        spdlog::info("Connection incoming...");
    }

    void handle();

    void disconnect();

private:
    enum {
        Start,
        Handle,
        Write,
        KeepRead,
        KeepHandle
    } connectionState = Start;

    void readSocket();

    void doHandle();

    void writeSocket();

    void keepReadSocket();

    void keepHandle();

    boost::asio::ip::tcp::socket socket;

    ConnectionPool& poolHandler;

    ThreadPool& queueHandler;

    HTTPRequest request;

    HTTPResponse response;

    std::array<char, 8192> buffer;

    std::size_t receiveLength;
};

using ConnectionPtr = std::shared_ptr<Connection>;

#endif //WEBIMG_CONNECTION_H
