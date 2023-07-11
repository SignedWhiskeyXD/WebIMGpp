//
// Created by wsmrxd on 2023/7/11.
//

#ifndef WEBIMG_MYSQLSESSION_H
#define WEBIMG_MYSQLSESSION_H

#include <boost/mysql.hpp>
#include "WebIMGppUser.h"

class MySQLSession {
    friend class SQLSessionPool;
public:
    MySQLSession(boost::asio::io_context& ctx, boost::asio::ssl::context& sslCtx, SQLSessionPool& pool):
            sqlConnection(ctx, sslCtx), poolHandler(pool) {}

    void test(const char *name, boost::mysql::results& result);

    bool checkUsername(std::string_view username);

    WebIMGppUser selectUser(std::string_view username);

    bool addUser(std::string_view username, std::string_view password);

private:
    boost::mysql::statement blockingPrepare(std::string_view sql);

    SQLSessionPool& poolHandler;

    boost::mysql::tcp_ssl_connection sqlConnection;

    boost::mysql::diagnostics diag;
};

using MySQLPtr = std::shared_ptr<MySQLSession>;


#endif //WEBIMG_MYSQLSESSION_H
