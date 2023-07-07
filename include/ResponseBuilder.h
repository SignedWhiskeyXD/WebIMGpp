//
// Created by wsmrxd on 2023/7/7.
//

#ifndef WEBIMG_RESPONSEBUILDER_H
#define WEBIMG_RESPONSEBUILDER_H

#include "HTTPRequest.h"
#include "HTTPResponse.h"

class ResponseBuilder {
public:
    explicit ResponseBuilder(RequestPtr requestPtr): request(std::move(requestPtr)) {}

    ResponseBuilder() = delete;

    ResponseBuilder(const ResponseBuilder&) = default;

    ResponseBuilder& operator=(const ResponseBuilder&) = default;

    HTTPResponse getResponse();

private:
    static void setFirstLineOK(HTTPResponse& response);

    static void setDefaultHeaders(HTTPResponse& response);

    RequestPtr request;
};


#endif //WEBIMG_RESPONSEBUILDER_H
