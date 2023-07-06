//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_CONNECTION_H
#define WEBIMG_CONNECTION_H

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

class Connection{
public:
    explicit Connection(boost::asio::ip::tcp::socket sock):
        socket(std::move(sock)) { }

    Connection() = delete;

    Connection(const Connection&) = delete;

    Connection& operator=(const Connection&) = delete;

    void info(){
        spdlog::info("Connection incoming...");
    }

private:
    boost::asio::ip::tcp::socket socket;
};

#endif //WEBIMG_CONNECTION_H
