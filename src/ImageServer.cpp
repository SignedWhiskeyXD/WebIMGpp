//
// Created by wsmrxd on 2023/7/6.
//

#include <spdlog/spdlog.h>
#include "ImageServer.h"
#include "Connection.h"
#include "Servlet/HelloServlet.h"
#include "Servlet/Register.h"
#include "Servlet/Login.h"
#include "Servlet/Upload.h"
#include "ServletMatcher.h"

ImageServer::ImageServer(std::string_view address, uint16_t port, uint16_t threadNum, uint16_t sqlNum) :
        ioContext(1),
        acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
        threadPool(true, threadNum),
        sqlSessionPool(address, boost::mysql::handshake_params("root", "wsmrxd", "db1"), sqlNum)
{
    ServletPtr hello = std::make_shared<HelloServlet>();
    ServletPtr regi = std::make_shared<Register>(sqlSessionPool);
    ServletPtr login = std::make_shared<Login>(sqlSessionPool);
    ServletPtr upload = std::make_shared<Upload>();

    ServletMatcher::getInstance().addRule("/hello", hello);
    ServletMatcher::getInstance().addRule("/register", regi);
    ServletMatcher::getInstance().addRule("/login", login);
    ServletMatcher::getInstance().addRule("/upload", upload);
    do_accept();
}

void ImageServer::run() {
    if(acceptor.is_open()) {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        spdlog::info("starting at {}", ss.str());
        threadPool.init();
        ioContext.run();
    }
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
                    (std::move(socket), connectionPool, threadPool);
                connectionPool.addConnection(newConnection);
                spdlog::info("Accepted, num of connections: {}", connectionPool.size());
                threadPool.commit(newConnection);
            }

            do_accept();
        });
}
