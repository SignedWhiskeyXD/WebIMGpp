//
// Created by wsmrxd on 2023/7/11.
//

#include "SQLSessionPool.h"

SQLSessionPool::SQLSessionPool(std::string_view host,
                               const boost::mysql::handshake_params &params, unsigned int num) :
        hostIP(host), parameters(params),
        sslContext(boost::asio::ssl::context::tls_client),
        connectionNum(num), semaphore(num)
{
    sessions.reserve(num);
    init();
}

void SQLSessionPool::init() noexcept {
    boost::asio::ip::tcp::resolver resolver(ioContext.get_executor());
    auto eps = resolver.resolve(hostIP.data(), boost::mysql::default_port_string);
    while(sessions.size() < connectionNum){
        sessions.emplace_back(std::make_shared<MySQLSession>(ioContext, sslContext, *this));
        sessions.back()->sqlConnection.connect(*eps.begin(), parameters);
    }
}

MySQLPtr SQLSessionPool::waitPop() {
    semaphore.acquire();

    Guard guard(mtx);
    MySQLPtr ret = std::move(sessions.back());
    sessions.pop_back();
    return ret;
}

void SQLSessionPool::release(MySQLPtr session) {
    {
        Guard guard(mtx);
        sessions.push_back(std::move(session));
    }
    semaphore.release(1);
}

