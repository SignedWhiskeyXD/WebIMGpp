//
// Created by wsmrxd on 2023/7/11.
//

#ifndef WEBIMG_ROOTSERVLET_H
#define WEBIMG_ROOTSERVLET_H

#include "ServletBase.h"
#include "ResponseBuilder.h"

class RootServlet : public ServletBase{
public:
    HTTPResponse doDefault(HTTPRequest &request) const override{
        return ResponseBuilder()
            .setStatus(404, "Not Found")
            .setPayload("Nothing to be shown here!").build();
    }
};

#endif //WEBIMG_ROOTSERVLET_H
