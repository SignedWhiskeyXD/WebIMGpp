//
// Created by wsmrxd on 2023/7/7.
//

#include "ResponseBuilder.h"
#include <iomanip>
#include <ctime>

ResponseBuilder &ResponseBuilder::setOK() {
    response.statusCode = "200";
    response.statusMessage = "OK";
    return *this;
}

ResponseBuilder &ResponseBuilder::setFirstLine(std::string_view firstLineNoCRLF) {
    response.firstLineWithCRLF = firstLineNoCRLF;
    response.firstLineWithCRLF += "\r\n";
    return *this;
}

ResponseBuilder &ResponseBuilder::setStatus(uint16_t code, std::string_view message) {
    response.statusCode = std::to_string(code);
    response.statusMessage = message;
    return *this;
}

ResponseBuilder &ResponseBuilder::setHeader(const std::pair<std::string, std::string>& header) {
    response.headers.insert(header);
    return *this;
}

ResponseBuilder &ResponseBuilder::setHeader(std::string_view key, std::string_view val) {
    response.headers[key.data()] = val;
    return *this;
}

ResponseBuilder &ResponseBuilder::setPayload(const std::string &payload) {
    response.payload = payload;
    return *this;
}

std::string ResponseBuilder::toString() const {
    std::string ret;
    if(response.firstLineWithCRLF.empty()){
        ret += response.httpVersion + " "
            + response.statusCode + " "
            + response.statusMessage + "\r\n";
    }else
        ret += response.firstLineWithCRLF;

    for(const auto& pa : response.headers)
        ret += pa.first + ": " + pa.second + "\r\n";

    ret += "\r\n";
    ret += response.payload;

    return ret;
}

ResponseBuilder &ResponseBuilder::contentType(std::string_view contentType) {
    setHeader("Content-Type", contentType);
    return *this;
}

HTTPResponse ResponseBuilder::build() {
    setContentLength();
    setTime();
    setServerName();

    if(!response.headers.contains("Content-Type"))
        setHeader("Content-Type", "text/plain");

    return response;
}

void ResponseBuilder::setContentLength() {
    setHeader("Content-Length", std::to_string(response.payload.length()));
}

void ResponseBuilder::setTime() {
    static const char* timeFormat = "%a, %d %b %Y %H:%M:%S GMT";
    std::time_t currentTime = std::time(nullptr);
    std::tm* timeInfo = std::gmtime(&currentTime);

    std::stringstream ss;
    ss << std::put_time(timeInfo, timeFormat);
    setHeader("Date", ss.str());
}

void ResponseBuilder::setServerName() {
    setHeader("Server", "WebIMGpp Experimental");
}

HTTPResponse ResponseBuilder::BadRequest() {
    static const char* payload400 = "<!DOCTYPE html>\n"
                                    "<html>\n"
                                    "<head>\n"
                                    "  <title>400 Bad Request</title>\n"
                                    "</head>\n"
                                    "<body>\n"
                                    "  <h1>400 Bad Request</h1>\n"
                                    "  <p>Invalid Request</p>\n"
                                    "</body>\n"
                                    "</html>";
    setStatus(400, "Bad Request");
    contentType("text/html");
    setPayload(payload400);
    return response;
}

HTTPResponse ResponseBuilder::NotFound() {
    static const char* payload404 = "<!DOCTYPE html>\n"
                                    "<html>\n"
                                    "<head>\n"
                                    "  <title>404 Not Found</title>\n"
                                    "</head>\n"
                                    "<body>\n"
                                    "  <h1>404 Not Found</h1>\n"
                                    "  <p>404 Not Found</p>\n"
                                    "</body>\n"
                                    "</html>";
    setStatus(400, "Bad Request");
    contentType("text/html");
    setPayload(payload404);
    return response;
}


