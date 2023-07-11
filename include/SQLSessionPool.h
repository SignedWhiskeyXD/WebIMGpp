//
// Created by wsmrxd on 2023/7/11.
//

#ifndef WEBIMG_SQLSESSIONPOOL_H
#define WEBIMG_SQLSESSIONPOOL_H

#include <boost/asio.hpp>
#include <semaphore>
#include "MySQLSession.h"

class SQLSessionPool {
public:
    SQLSessionPool(std::string_view host,
                   const boost::mysql::handshake_params& params, unsigned int num);

    MySQLPtr waitPop();

    void release(MySQLPtr session);

    std::size_t available() const noexcept{
        Guard guard(mtx);
        return sessions.size();
    }

private:
    void init() noexcept;

    boost::asio::io_context ioContext;

    boost::asio::ssl::context sslContext;

    std::vector<MySQLPtr> sessions;

    std::counting_semaphore<20> semaphore;

    mutable std::mutex mtx;

    std::string_view hostIP;

    boost::mysql::handshake_params parameters;

    const unsigned int connectionNum;

    using Guard = std::lock_guard<std::mutex>;
};


#endif //WEBIMG_SQLSESSIONPOOL_H
