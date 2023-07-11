//
// Created by wsmrxd on 2023/7/11.
//

#ifndef WEBIMG_LOGIN_H
#define WEBIMG_LOGIN_H

#include "ServletBase.h"
#include "SQLSessionPool.h"

class Login : public ServletBase {
public:
    explicit Login(SQLSessionPool& pool) : poolHandler(pool) {}

    HTTPResponse doPost(HTTPRequest &request) const override;

private:
    SQLSessionPool& poolHandler;
};


#endif //WEBIMG_LOGIN_H
