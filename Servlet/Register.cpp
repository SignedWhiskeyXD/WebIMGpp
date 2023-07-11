//
// Created by wsmrxd on 2023/7/11.
//

#include "Register.h"
#include "ResponseBuilder.h"
#include "RequestParser.h"

HTTPResponse Register::doPost(HTTPRequest &request) const {
    ResponseBuilder builder;
    auto form = RequestParser::parseForm(request.payload);
    if(!form.contains("user") || !form.contains("password") || request.uri != "/register"){
        return builder.BadRequest();
    }
    std::string username = form["user"], password = form["password"];
    auto sqlSession = poolHandler.waitPop();
    if(sqlSession->checkUsername(username))
        return builder.setStatus(400, "Bad Request").setPayload("Username already exists!").build();

    bool result = sqlSession->addUser(username, password);
    poolHandler.release(sqlSession);

    if(result)
        return builder.setOK().setPayload("Registration success!").build();
    else
        return builder.setStatus(500, "Internal Server Error").setPayload("SQL Error").build();
}
