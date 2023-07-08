//
// Created by wsmrxd on 2023/7/6.
//

#include <spdlog/spdlog.h>
#include "ImageServer.h"
#include "Connection.h"

ImageServer::ImageServer(std::string_view address, uint16_t port):
        ioContext(1),
        acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
        threadPool(true)
{
    do_accept();
}

void ImageServer::run() {
    if(acceptor.is_open())
        spdlog::info("starting");
    threadPool.init(8);
    ioContext.run();
}

void ImageServer::stop() {
    acceptor.close();
    threadPool.forceStop();
}

void ImageServer::do_accept() {
    acceptor.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (!acceptor.is_open()) return;

            if (!ec){
                auto newConnection = std::make_shared<Connection>
                    (std::move(socket), connectionPool);
                connectionPool.addConnection(newConnection);
                threadPool.commit(newConnection);
            }

            do_accept();
        });
}
