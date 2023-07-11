//
// Created by wsmrxd on 2023/7/11.
//

#include <catch2/catch_test_macros.hpp>
#include "SQLSessionPool.h"

TEST_CASE("testSQLSessionPool")
{
    boost::asio::io_context ioContext;

    boost::mysql::handshake_params params("root", "wsmrxd", "db1");

    boost::mysql::results results;

    SQLSessionPool pool(ioContext, "127.0.0.1", params, 4);

    auto& session = pool.get();

    session.test("Tadokoro", results);

    ioContext.run();

    REQUIRE(results.rows().at(0).at(2).get_string() == "114514");
}