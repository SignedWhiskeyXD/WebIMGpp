//
// Created by wsmrxd on 2023/7/9.
//

#ifndef WEBIMG_SERVLETBASE_H
#define WEBIMG_SERVLETBASE_H

#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "ResponseBuilder.h"

class ServletBase{
public:
    ServletBase() = default;

    HTTPResponse onHandle(HTTPRequest& request) const {
        if(request.httpVersion != "HTTP/1.1") doDefault(request);
        if(request.method == "GET") return doGet(request);
        if(request.method == "POST") return doPost(request);
        return doDefault(request);
    }

protected:
    virtual HTTPResponse doGet(HTTPRequest& request) const {
        return doDefault(request);
    }

    virtual HTTPResponse doPost(HTTPRequest& request) const {
        return doDefault(request);
    }

    virtual HTTPResponse doDefault(HTTPRequest& request) const {
        return ResponseBuilder().setStatus(400, "Bad Request").setPayload("Invalid Request!").build();
    }
};

using ServletPtr = std::shared_ptr<ServletBase>;

#endif //WEBIMG_SERVLETBASE_H
