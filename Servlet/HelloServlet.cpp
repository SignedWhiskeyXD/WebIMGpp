//
// Created by wsmrxd on 2023/7/9.
//
#include "HelloServlet.h"
#include <spdlog/spdlog.h>

HTTPResponse HelloServlet::doGet(HTTPRequest& request) const {
    ResponseBuilder builder;
    if(request.uri == "/hello"){
        std::string payload = "Welcome to WebIMGpp experimental!";
        builder.setOK().setPayload(payload).contentType("text/plain");
    }else{
        return builder.BadRequest();
    }
    return builder.build();
}

HTTPResponse HelloServlet::doPost(HTTPRequest &request) const {
    ResponseBuilder builder;
    if(request.uri == "/hello"){
        std::string payload = "Form received, Welcome to WebIMGpp experimental!";
        spdlog::info("Received POST request:\n{}", request.payload);
        builder.setOK().setPayload(payload).contentType("text/plain");
    }else{
        return builder.BadRequest();
    }
    return builder.build();
}

