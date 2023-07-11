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

    std::string test();

private:
    boost::mysql::tcp_ssl_connection sqlConnection;
};


#endif //WEBIMG_MYSQLSESSION_H
