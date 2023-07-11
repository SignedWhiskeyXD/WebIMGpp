//
// Created by wsmrxd on 2023/7/11.
//

#include "Register.h"
#include "ResponseBuilder.h"
#include "RequestParser.h"

HTTPResponse Register::doPost(HTTPRequest &request) const {
    ResponseBuilder builder;
    if(request.uri != "login") return builder.NotFound();

    builder.setOK().setPayload("OK");
    auto form = RequestParser::parseForm(request.payload);

    return builder.build();
}
