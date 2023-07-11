//
// Created by wsmrxd on 2023/7/11.
//

#include <catch2/catch_test_macros.hpp>
#include "SQLSessionPool.h"

TEST_CASE("testSQLSessionPool")
{
    boost::asio::io_context ioContext;

    boost::mysql::handshake_params params("root", "wsmrxd", "db1");

    boost::mysql::results results1, results2;

    SQLSessionPool pool(ioContext, "127.0.0.1", params, 4);

    MySQLPtr session1 = pool.waitPop(), session2 = pool.waitPop();

    session1->test("Tadokoro", results1);
    session2->test("wsmrxd",results2);

    ioContext.run();

    pool.release(session1);
    pool.release(session2);

    REQUIRE(results1.rows().at(0).at(2).get_string() == "114514");
    REQUIRE(results2.rows().at(0).at(3).get_string() == "Wuhan");
    REQUIRE(pool.available() == 4);
}