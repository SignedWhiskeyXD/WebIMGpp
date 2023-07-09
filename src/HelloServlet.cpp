//
// Created by wsmrxd on 2023/7/9.
//
#include "HelloServlet.h"

HTTPResponse HelloServlet::onHandle(HTTPRequest& request) {
    HTTPResponse response;
    response.setDefaultHeaders();

    if(request.method == "GET" && request.uri == "/hello" && request.httpVersion == "HTTP/1.1"){
        response.setFirstLineOK();
        response.payload = "Welcome to WebIMGpp experimental! ";
    }else{
        response.status = "400";
        response.isOK = "Bad Request";
        response.payload = "<!DOCTYPE html>\n"
                           "<html>\n"
                           "<head>\n"
                           "  <title>400 Bad Request</title>\n"
                           "</head>\n"
                           "<body>\n"
                           "  <h1>400 Bad Request</h1>\n"
                           "  <p>Invalid Request</p>\n"
                           "</body>\n"
                           "</html>";
    }
    return response;
}

