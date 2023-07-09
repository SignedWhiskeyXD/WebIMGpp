//
// Created by wsmrxd on 2023/7/7.
//

#ifndef WEBIMG_RESPONSEBUILDER_H
#define WEBIMG_RESPONSEBUILDER_H

#include "HTTPResponse.h"

class ResponseBuilder {
public:
    ResponseBuilder() = default;

    ResponseBuilder(const ResponseBuilder&) = default;

    ResponseBuilder& operator=(const ResponseBuilder&) = default;

    ResponseBuilder& setOK();

    ResponseBuilder& setStatus(uint16_t code, std::string_view message);

    ResponseBuilder& setFirstLine(std::string_view firstLine);

    ResponseBuilder& setHeader(std::pair<std::string, std::string> header);

    ResponseBuilder& setHeader(std::string_view key, std::string_view val);

    ResponseBuilder& contentType(std::string_view contentType);

    ResponseBuilder& setPayload(const std::string& payload);

    std::string toString() const;

    HTTPResponse build();

    HTTPResponse operator()(){
        return build();
    }

private:
    void setContentLength();

    void setTime();

    void setServerName();

    HTTPResponse response;
};


#endif //WEBIMG_RESPONSEBUILDER_H
