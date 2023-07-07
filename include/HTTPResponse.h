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
};

using ResponsePtr = std::shared_ptr<HTTPResponse>;

#endif //WEBIMG_HTTPRESPONSE_H
