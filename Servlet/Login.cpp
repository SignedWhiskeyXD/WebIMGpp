//
// Created by wsmrxd on 2023/7/11.
//

#include "Login.h"
#include "ResponseBuilder.h"
#include "RequestParser.h"

HTTPResponse Login::doPost(HTTPRequest &request) const {
    ResponseBuilder builder;
    auto form = RequestParser::parseForm(request.payload);
    if(!form.contains("user") || !form.contains("password") || request.uri != "/login"){
        return builder.BadRequest();
    }
    std::string username = form["user"], password = form["password"].data();
    auto sqlSession = poolHandler.waitPop();
    auto result = sqlSession->selectUserByName(username);
    if(result == 1)
        return builder.setOK().setPayload("Access Granted!").build();
    else
        return builder.setOK().setPayload("Invalid user!").build();
}
