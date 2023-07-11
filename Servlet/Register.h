//
// Created by wsmrxd on 2023/7/11.
//

#ifndef WEBIMG_REGISTER_H
#define WEBIMG_REGISTER_H

#include "ServletBase.h"

class Register : public ServletBase{
public:
    HTTPResponse doPost(HTTPRequest &request) const override;
};


#endif //WEBIMG_REGISTER_H
