//
// Created by wsmrxd on 2023/7/11.
//

#include <catch2/catch_test_macros.hpp>
#include "SQLSessionPool.h"

TEST_CASE("testSQLSessionPool")
{
    boost::asio::io_context ioContext;

    boost::mysql::handshake_params params("root", "wsmrxd", "db1");

    SQLSessionPool pool(ioContext, "127.0.0.1", params, 4);

    ioContext.run();

    auto res = pool.test();

    REQUIRE(res == "Tadokoro");
}