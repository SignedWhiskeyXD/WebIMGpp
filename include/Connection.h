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

class Connection : public std::enable_shared_from_this<Connection>{
public:
    friend class RequestParser;

    explicit Connection(boost::asio::ip::tcp::socket sock, ConnectionPool& pool):
        socket(std::move(sock)), poolHandler(pool) { }

    Connection() = delete;

    Connection(const Connection&) = delete;

    Connection& operator=(const Connection&) = delete;

    void info(){
        spdlog::info("Connection incoming...");
    }

    void handle();

    void disconnect();

private:
    void readSocket();

    void writeSocket();

    boost::asio::ip::tcp::socket socket;

    ConnectionPool& poolHandler;

    HTTPRequest request;

    HTTPResponse response;

    std::array<char, 8192> buffer;
};

using ConnectionPtr = std::shared_ptr<Connection>;

#endif //WEBIMG_CONNECTION_H
