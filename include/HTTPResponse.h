//
// Created by wsmrxd on 2023/7/7.
//

#ifndef WEBIMG_HTTPRESPONSE_H
#define WEBIMG_HTTPRESPONSE_H

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <boost/asio.hpp>

struct HTTPResponse{
    std::string httpVersion;
    int status;
    std::string isOK;
    std::map<std::string, std::string> headers;

    std::string payload;

    std::vector<boost::asio::const_buffer> toAsioBuffers();
};

inline std::vector<boost::asio::const_buffer> HTTPResponse::toAsioBuffers() {
    static const std::string firstLineOK = "HTTP/1.1 200 OK\r\n";
    static const std::string betweenHeader = ": ";
    static const std::string newLine = "\r\n";
    using boost::asio::buffer;

    std::vector<boost::asio::const_buffer> ret;
    ret.emplace_back(buffer(firstLineOK));

    for(const auto& header : headers) {
        ret.emplace_back(buffer(header.first));
        ret.emplace_back(buffer(betweenHeader));
        ret.emplace_back(buffer(header.second));
        ret.emplace_back(buffer(newLine));
    }
    ret.emplace_back(buffer(newLine));
    ret.emplace_back(buffer(payload));

    return ret;
}

using ResponsePtr = std::shared_ptr<HTTPResponse>;

#endif //WEBIMG_HTTPRESPONSE_H
