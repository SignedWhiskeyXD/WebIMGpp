//
// Created by wsmrxd on 2023/7/6.
//

#include "RequestParser.h"

std::pair<int, HTTPRequest> RequestParser::parseRequest(std::stringstream &requestStream) {
    enum parseState{
        FIRST_LINE = 0,
        HEADERS,
        PAYLOAD
    };
    auto ret = std::make_pair<int, HTTPRequest>(-1, HTTPRequest());

    int currentState = parseState::FIRST_LINE;
    std::string line;
    while(currentState < PAYLOAD && std::getline(requestStream, line)){
        switch (currentState) {
            case FIRST_LINE:{
                int flag = parseFirstLine(ret.second, line);
                if(flag == GOOD_FIRST_LINE) ++currentState;
                else return ret;
                break;
            }
            case HEADERS:{
                int flag = parseHeaders(ret.second, line);
                if(flag == CONTINUE) continue;
                else if(flag == GOOD_HEADERS)
                    ++currentState;
                break;
            }
            default:{
                break;
            }
        }
    }
    return ret;
}

int RequestParser::parseFirstLine(HTTPRequest &parsedRequest, std::string_view lineView) {
    std::stringstream lineStream(lineView.data());
    lineStream >> parsedRequest.method
        >> parsedRequest.uri
        >> parsedRequest.httpVersion;

    return !parsedRequest.method.empty() &&
           !parsedRequest.uri.empty() &&
           !parsedRequest.httpVersion.empty() ? GOOD_FIRST_LINE : BAD_METHOD;
}

int RequestParser::parseHeaders(HTTPRequest &parsedRequest, std::string_view lineView) {
    if(lineView.length() < 5) return GOOD_HEADERS;
    auto currentHeader = std::make_pair<std::string, std::string>("", "");

    auto div = lineView.find(": ");
    if(div != std::string::npos && lineView.begin() + div + 2 < lineView.end() - 1) {
        currentHeader.first = std::string(lineView.begin(), lineView.begin() + div);
        currentHeader.second = std::string(lineView.begin() + div + 2, lineView.end() - 1);
        parsedRequest.headers.insert(currentHeader);
    }

    return !currentHeader.first.empty() &&
           !currentHeader.second.empty() ? CONTINUE : BAD_HEADER_CONTINUE;
}