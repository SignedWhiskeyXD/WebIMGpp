//
// Created by wsmrxd on 2023/7/11.
//

#ifndef WEBIMG_REGISTER_H
#define WEBIMG_REGISTER_H

#include "ServletBase.h"
#include "SQLSessionPool.h"

class Register : public ServletBase{
public:
    explicit Register(SQLSessionPool& pool) : poolHandler(pool) {}

    HTTPResponse doPost(HTTPRequest &request) const override;

private:
    SQLSessionPool& poolHandler;
};


#endif //WEBIMG_REGISTER_H
