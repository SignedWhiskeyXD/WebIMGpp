//
// Created by wsmrxd on 2023/7/9.
//

#ifndef WEBIMG_SERVLETBASE_H
#define WEBIMG_SERVLETBASE_H

#include "HTTPRequest.h"
#include "HTTPResponse.h"

class ServletBase{
public:
    virtual HTTPResponse onHandle(HTTPRequest& request) = 0;
};

using ServletPtr = std::shared_ptr<ServletBase>;

#endif //WEBIMG_SERVLETBASE_H
