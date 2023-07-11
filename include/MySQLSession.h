//
// Created by wsmrxd on 2023/7/11.
//

#ifndef WEBIMG_MYSQLSESSION_H
#define WEBIMG_MYSQLSESSION_H

#include <boost/mysql.hpp>

class MySQLSession {
    friend class SQLSessionPool;
public:
    MySQLSession(boost::asio::io_context& ctx, boost::asio::ssl::context& sslCtx):
            sqlConnection(ctx, sslCtx) {}

    void test(const char *name, boost::mysql::results& result);

private:
    void executeTest(const char *name, boost::mysql::results& result);

    boost::mysql::tcp_ssl_connection sqlConnection;

    boost::mysql::diagnostics diag;

    boost::mysql::statement nextStmt;
};


#endif //WEBIMG_MYSQLSESSION_H
