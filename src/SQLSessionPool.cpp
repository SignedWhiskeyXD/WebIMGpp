//
// Created by wsmrxd on 2023/7/11.
//

#include "SQLSessionPool.h"

SQLSessionPool::SQLSessionPool(boost::asio::io_context &ctx, std::string_view host,
                               const boost::mysql::handshake_params &params, unsigned int num) :
        ioContext(ctx), hostIP(host), parameters(params),
        sslContext(boost::asio::ssl::context::tls_client),
        resolver(ctx.get_executor()), connectionNum(num)

{
    resolveHost();
}

void SQLSessionPool::resolveHost() {
    resolver.async_resolve(
        hostIP.data(),
        boost::mysql::default_port_string,
        [this](boost::system::error_code ec,
               boost::asio::ip::tcp::resolver::results_type results){
            boost::mysql::throw_on_error(ec);
            eps = std::move(results);
            connectAll();
        });
}

void SQLSessionPool::connectAll() {
    while(sessions.size() < connectionNum){
        sessions.emplace_back(ioContext, sslContext);
        sessions.back().sqlConnection.async_connect(*eps.begin(), parameters, diag,
            [this](boost::system::error_code ec){
                boost::mysql::throw_on_error(ec, diag);
            });
    }
}

