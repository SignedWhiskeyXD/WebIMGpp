//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_CONNECTION_H
#define WEBIMG_CONNECTION_H

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include "HTTPResponse.h"
#include "HTTPRequest.h"

class Connection{
public:
    friend class RequestParser;

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

    HTTPRequest request;

    HTTPResponse response;

    std::vector<boost::asio::const_buffer> toAsioBuffers(){
        auto ret = std::vector<boost::asio::const_buffer>();

        ret.emplace_back(boost::asio::buffer(response.httpVersion + " " +
                                          std::to_string(response.status) + " "
                                          + response.isOK + "\r\n"));
        for(const auto& header : response.headers)
            ret.emplace_back(boost::asio::buffer(header.first + ": " + header.second + "\r\n"));
        ret.emplace_back(boost::asio::buffer("\r\n"));
        ret.emplace_back(boost::asio::buffer(response.payload));

        return ret;
    }
};

using ConnectionPtr = std::shared_ptr<Connection>;

#endif //WEBIMG_CONNECTION_H
