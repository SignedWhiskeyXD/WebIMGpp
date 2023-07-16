//
// Created by wsmrxd on 2023/7/16.
//

#include "Upload.h"
#include <spdlog/spdlog.h>
#include <fstream>

HTTPResponse Upload::doPost(HTTPRequest &request) const {
    ResponseBuilder builder;
    if(request.uri == "/upload"){
        std::string payload = "Upload file received";
        auto boundaries = parseBoundary(request);

        for(std::size_t i = 0; i + 1 < boundaries.size(); ++i){
            if(request.payload.find("filename=") == std::string::npos) break;

            std::size_t fileNameStart = request.payload.find("filename=") + 10;
            std::size_t fileNameEnd = request.payload.find('\"', fileNameStart);
            std::size_t fileStart = request.payload.find("\r\n\r\n", boundaries[i]) + 4;
            std::size_t fileEnd = boundaries[i + 1] - 2;

            if(fileNameStart >= fileNameEnd || fileStart >= fileEnd)
                break;

            auto fileName = std::string(request.payload.begin() + fileNameStart,
                                        request.payload.begin() + fileNameEnd);

            auto fileData = std::string(request.payload.begin() + fileStart,
                                        request.payload.begin() + fileEnd);

            std::ofstream outputFile(fileName.c_str(), std::ios::binary | std::ios::out);
            outputFile.write(fileData.c_str(), fileData.size());
        }

        builder.setOK().setPayload(payload).contentType("text/plain");
    }else{
        return builder.BadRequest();
    }
    return builder.build();
}

std::vector<std::size_t> Upload::parseBoundary(HTTPRequest &request) {
    static const char* boundary = "------WebKitFormBoundary";

    std::vector<std::size_t> ret;
    ret.push_back(request.payload.find(boundary));
    while(ret.back() != std::string::npos){
        ret.push_back(request.payload.find(boundary, ret.back() + 1));
    }
    ret.pop_back();
    return ret;
}
