//
// Created by wsmrxd on 2023/7/9.
//

#ifndef WEBIMG_IMGSERVLET_H
#define WEBIMG_IMGSERVLET_H

#include "HTTPRequest.h"
#include "HTTPResponse.h"

class IMGServlet{
public:
    virtual HTTPResponse onHandle(HTTPRequest& request) = 0;
};

using ServletPtr = std::shared_ptr<IMGServlet>;

#endif //WEBIMG_IMGSERVLET_H
