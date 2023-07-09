//
// Created by wsmrxd on 2023/7/9.
//
#include "HelloServlet.h"

HTTPResponse HelloServlet::onHandle(HTTPRequest& request) {
    if(request.method == "GET" && request.uri == "/hello" && request.httpVersion == "HTTP/1.1"){
        std::string payload = "Welcome to WebIMGpp experimental!";
        builder.setOK().setPayload(payload).contentType("text/plain");
    }else{
        std::string payload = "<!DOCTYPE html>\n"
                              "<html>\n"
                              "<head>\n"
                              "  <title>400 Bad Request</title>\n"
                              "</head>\n"
                              "<body>\n"
                              "  <h1>400 Bad Request</h1>\n"
                              "  <p>Invalid Request</p>\n"
                              "</body>\n"
                              "</html>";
        builder.setStatus(400, "Bad Request").setPayload(payload).contentType("text/html");
    }
    return builder.build();
}

