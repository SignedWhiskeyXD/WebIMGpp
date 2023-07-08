//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_REQUESTPARSER_H
#define WEBIMG_REQUESTPARSER_H

#include <unordered_set>
#include "Connection.h"
#include "HTTPRequest.h"
#include "ResponseBuilder.h"

class RequestParser {
public:

    static std::pair<int, HTTPRequest> parseRequest(std::istream& requestStream);

private:
    static int parseFirstLine(HTTPRequest &parsedRequest, std::stringstream &lineStream);

    static int parseHeaders(HTTPRequest &parsedRequest, std::stringstream &lineStream);

    enum{
        BAD_METHOD,
        BAD_URI,
        BAD_HTTP_VERSION,
        GOOD_FIRST_LINE,

        CONTINUE,
        BAD_HEADER_CONTINUE,
        GOOD_HEADERS
    };
};


#endif //WEBIMG_REQUESTPARSER_H
