//
// Created by wsmrxd on 2023/7/9.
//

#ifndef WEBIMG_HELLOSERVLET_H
#define WEBIMG_HELLOSERVLET_H

#include "IMGServlet.h"

class HelloServlet : public IMGServlet{
public:

    HTTPResponse onHandle(HTTPRequest& request) override;


};

#endif //WEBIMG_HELLOSERVLET_H
