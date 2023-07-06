//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_REQUESTPARSER_H
#define WEBIMG_REQUESTPARSER_H

#include <string_view>
#include "Connection.h"
#include "HTTPRequest.h"

class RequestParser {
public:
    explicit RequestParser(ConnectionPtr conn, std::string_view api_uri = "/api"):
        connection(std::move(conn)), uri(api_uri), buffer() {}

    HTTPRequest handle();

    static std::pair<int, HTTPRequest> parseRequest(std::istream& requestStream);

private:
    void readSocket();

    static bool isValidFirstLine(HTTPRequest& request);

    std::string uri;

    ConnectionPtr connection;

    enum{
        BAD_METHOD,
        BAD_URI,
        BAD_HTTP_VERSION,
        GOOD_FIRST_LINE,

        CONTINUE,
        BAD_HEADER_CONTINUE,
        GOOD_HEADERS
    };

    std::array<char, 8192> buffer;

    static int parseFirstLine(HTTPRequest &parsedRequest, std::stringstream &lineStream);

    static int parseHeaders(HTTPRequest &parsedRequest, std::stringstream &lineStream);
};


#endif //WEBIMG_REQUESTPARSER_H
