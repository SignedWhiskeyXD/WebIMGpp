//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_HTTPREQUEST_H
#define WEBIMG_HTTPREQUEST_H

#include <string>
#include <map>
#include <memory>

struct HTTPRequest{
    std::string method;
    std::string uri;
    std::string httpVersion;
    std::map<std::string, std::string> headers;
    std::string payload;
};

using RequestPtr = std::shared_ptr<HTTPRequest>;

using RequestHeader = std::map<std::string, std::string>;

#endif //WEBIMG_HTTPREQUEST_H
