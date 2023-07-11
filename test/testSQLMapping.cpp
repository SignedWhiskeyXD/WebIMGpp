//
// Created by wsmrxd on 2023/7/11.
//

#include <catch2/catch_test_macros.hpp>
#include "SQLSessionPool.h"

TEST_CASE("testSQLMapping")
{
    boost::mysql::results results1, results2;

    SQLSessionPool pool("127.0.0.1",
                        boost::mysql::handshake_params("root", "wsmrxd", "db1"), 4);

    MySQLPtr session = pool.waitPop();

    auto rows = session->checkUsername("wsmrxd");

    pool.release(session);

    REQUIRE(rows == 1);
}