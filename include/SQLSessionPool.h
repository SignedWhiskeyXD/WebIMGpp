//
// Created by wsmrxd on 2023/7/11.
//

#ifndef WEBIMG_SQLSESSIONPOOL_H
#define WEBIMG_SQLSESSIONPOOL_H

#include <boost/asio.hpp>
#include "MySQLSession.h"

class SQLSessionPool {
public:
    SQLSessionPool(boost::asio::io_context& ctx, std::string_view host,
                   const boost::mysql::handshake_params& params, unsigned int num);

    MySQLSession& get(){
        return sessions.front();
    }

private:
    void resolveHost();

    void connectAll();

    boost::asio::io_context& ioContext;

    boost::asio::ssl::context sslContext;

    boost::asio::ip::tcp::resolver resolver;

    boost::asio::ip::tcp::resolver::results_type eps;

    std::vector<MySQLSession> sessions;

    std::string_view hostIP;

    boost::mysql::handshake_params parameters;

    const unsigned int connectionNum;

    boost::mysql::diagnostics diag;
};


#endif //WEBIMG_SQLSESSIONPOOL_H
