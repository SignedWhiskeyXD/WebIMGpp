//
// Created by wsmrxd on 2023/7/6.
//

#include "RequestParser.h"
#include <spdlog/spdlog.h>

void RequestParser::readSocket() {
    auto self(this->connection);
    connection->socket.async_read_some(boost::asio::buffer(buffer),
        [this, self](boost::system::error_code ec, std::size_t length){
            if(ec) return;

            if(length > 8192 || length == 0){

                return;
            }

            std::string_view view(buffer.begin(), buffer.begin() + length);
            std::stringstream ss(view.data());
            auto [parseFlag, request] = parseRequest(ss);

            // TODO: 已经解析好请求，响应之
        });
}

HTTPRequest RequestParser::handle() {
    HTTPRequest request;
    readSocket();
    return request;
}

bool RequestParser::isValidFirstLine(HTTPRequest &request) {
    if(request.method.empty() || request.uri.empty() || request.httpVersion.empty())
        return false;
    return true;
}

std::pair<int, HTTPRequest> RequestParser::parseRequest(std::istream &requestStream) {
    enum parseState{
        FIRST_LINE = 0,
        HEADERS,
        PAYLOAD
    };
    auto ret = std::make_pair<int, HTTPRequest>(-1, HTTPRequest());

    int currentState = parseState::FIRST_LINE;
    std::string line;
    while(std::getline(requestStream, line)){
        std::stringstream lineStream(line);
        switch (currentState) {
            case FIRST_LINE:{
                int flag = parseFirstLine(ret.second, lineStream);
                if(flag == GOOD_FIRST_LINE) ++currentState;
                else return ret;
                break;
            }
            case HEADERS:{
                int flag = parseHeaders(ret.second, lineStream);
                if(flag == CONTINUE) continue;
                else if(flag == GOOD_HEADERS){
                    ret.first = 200;
                    return ret;
                }
                else return ret;
            }
            default:{
                break;
            }
        }
    }
}

int RequestParser::parseFirstLine(HTTPRequest &parsedRequest, std::stringstream &lineStream) {
    lineStream >> parsedRequest.method
               >> parsedRequest.uri
               >> parsedRequest.httpVersion;

    return !parsedRequest.method.empty() &&
           !parsedRequest.uri.empty() &&
           !parsedRequest.httpVersion.empty() ? GOOD_FIRST_LINE : BAD_METHOD;
}

int RequestParser::parseHeaders(HTTPRequest &parsedRequest, std::stringstream &lineStream) {
    if(lineStream.str().length() < 3) return GOOD_HEADERS;
    auto currentHeader = std::make_pair<std::string, std::string>("", "");

    lineStream >> currentHeader.first >> currentHeader.second;
    currentHeader.first.pop_back();
    parsedRequest.headers.insert(currentHeader);

    return !currentHeader.first.empty() &&
           !currentHeader.second.empty() ? CONTINUE : BAD_HEADER_CONTINUE;
}
