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
    std::string username = form["user"], password = form["password"];
    auto sqlSession = poolHandler.waitPop();
    if(!sqlSession->checkUsername(username))
        return builder.setStatus(401, "Unauthorized").setPayload("No such username!").build();

    const std::string correctPassword = sqlSession->selectUser(username).password;
    poolHandler.release(sqlSession);

    if(password == correctPassword)
        return builder.setOK().setPayload("Access Granted!").build();
    else
        return builder.setStatus(401, "Unauthorized").setPayload("Password Invalid!").build();
}
