//
// Created by wsmrxd on 2023/7/9.
//

#ifndef WEBIMG_HELLOSERVLET_H
#define WEBIMG_HELLOSERVLET_H

#include "ServletBase.h"
#include "ResponseBuilder.h"

class HelloServlet : public ServletBase{
public:
    HelloServlet() = default;

    HTTPResponse doGet(HTTPRequest& request) const override;

    HTTPResponse doPost(HTTPRequest &request) const override;
};

#endif //WEBIMG_HELLOSERVLET_H
