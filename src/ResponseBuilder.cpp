//
// Created by wsmrxd on 2023/7/7.
//

#include "ResponseBuilder.h"

HTTPResponse ResponseBuilder::getResponse() {
    HTTPResponse response;
    setFirstLineOK(response);
    setDefaultHeaders(response);

    if(request->uri.find("/hello") != request->uri.length())
        response.payload = "Hello from WebIMGpp! ";
    return response;
}

void ResponseBuilder::setFirstLineOK(HTTPResponse &response) {
    response.httpVersion = "HTTP/1.1";
    response.status = 200;
    response.isOK = "OK";
}

void ResponseBuilder::setDefaultHeaders(HTTPResponse &response) {
    static std::pair<std::string, std::string> defaultHeaders ={
            "Server", "WebIMGpp experimental"
    };
    response.headers.insert(defaultHeaders);
}