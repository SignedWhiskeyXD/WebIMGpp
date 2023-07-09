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
    std::string httpVersion = "HTTP/1.1";
    std::string statusCode;
    std::string statusMessage;
    std::string firstLineWithCRLF;
    std::map<std::string, std::string> headers;

    std::string payload;

    std::vector<boost::asio::const_buffer> toAsioBuffers();

    HTTPResponse& setFirstLineOK(){
        firstLineWithCRLF = "HTTP/1.1 200 OK\r\n";
        return *this;
    }

    HTTPResponse& setDefaultHeaders(){
        static std::map<std::string, std::string> defaultHeaders ={
                {"Server", "WebIMGpp experimental"},
                {"Connection", "Keep-Alive"}
        };

        for(const auto& pa : defaultHeaders)
            headers.insert(pa);
        return *this;
    }
};

inline std::vector<boost::asio::const_buffer> HTTPResponse::toAsioBuffers() {
    static const std::string space = " ";
    static const std::string betweenHeader = ": ";
    static const std::string CRLF = "\r\n";
    using boost::asio::buffer;

    std::vector<boost::asio::const_buffer> ret;
    if(firstLineWithCRLF.empty()){
        ret.emplace_back(buffer(httpVersion));
        ret.emplace_back(buffer(space));
        ret.emplace_back(buffer(statusCode));
        ret.emplace_back(buffer(space));
        ret.emplace_back(buffer(statusMessage));
        ret.emplace_back(buffer(CRLF));
    }else{
        ret.emplace_back(buffer(firstLineWithCRLF));
    }

    for(const auto& header : headers) {
        ret.emplace_back(buffer(header.first));
        ret.emplace_back(buffer(betweenHeader));
        ret.emplace_back(buffer(header.second));
        ret.emplace_back(buffer(CRLF));
    }
    ret.emplace_back(buffer(CRLF));
    ret.emplace_back(buffer(payload));

    return ret;
}

using ResponsePtr = std::shared_ptr<HTTPResponse>;

#endif //WEBIMG_HTTPRESPONSE_H
