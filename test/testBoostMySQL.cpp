//
// Created by wsmrxd on 2023/7/9.
//

#include <catch2/catch_test_macros.hpp>
#include <boost/mysql.hpp>
#include <boost/asio.hpp>

TEST_CASE("testBoostMySQL")
{
    boost::asio::io_context ctx;

    boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::tls_client);

    boost::mysql::tcp_ssl_connection conn(ctx.get_executor(), ssl_ctx);

    boost::asio::ip::tcp::resolver resolver(ctx.get_executor());

    auto endpoints = resolver.resolve("127.0.0.1", "3306");

    boost::mysql::handshake_params params("root", "wsmrxd", "db1");

    conn.connect(*endpoints.begin(), params);

    const char* sql = "SELECT * FROM tb_user";

    boost::mysql::results results;
    conn.query(sql, results);

    REQUIRE(results.rows().at(0).at(1).get_string() == "Tadokoro");

    conn.close();
}