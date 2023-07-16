//
// Created by wsmrxd on 2023/7/16.
//

#ifndef WEBIMG_UPLOAD_H
#define WEBIMG_UPLOAD_H

#include "ServletBase.h"

class Upload : public ServletBase {
public:
    Upload() = default;

    HTTPResponse doPost(HTTPRequest &request) const override;

private:
    static std::vector<std::size_t> parseBoundary(HTTPRequest &request);
};


#endif //WEBIMG_UPLOAD_H
