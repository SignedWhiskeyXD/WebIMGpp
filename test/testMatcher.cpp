//
// Created by wsmrxd on 2023/7/9.
//

#include <catch2/catch_test_macros.hpp>
#include "ImageServer.h"
#include "HelloServlet.h"
#include "ServletMatcher.h"

TEST_CASE("testMatcher")
{
    ServletPtr hello = std::make_shared<HelloServlet>();
    ServletMatcher matcher = ServletMatcher::getInstance();
    matcher.addRule("/hello", hello);

    HTTPRequest request1{"GET", "/hello", "HTTP/1.1"};
    HTTPRequest request2{"GET", "/hello/wtf_is_this", "HTTP/1.1"};

    auto servlet1 = matcher.match(request1);
    auto servlet2 = matcher.match(request2);

    REQUIRE(servlet1 == hello);
    REQUIRE(servlet2 == hello);
}
