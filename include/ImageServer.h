//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_IMAGESERVER_H
#define WEBIMG_IMAGESERVER_H

#include <boost/asio.hpp>
#include "ThreadPool.h"
#include "ConnectionPool.h"
#include "SQLSessionPool.h"

class ImageServer{
public:
    ImageServer(std::string_view address, uint16_t port);

    ImageServer() = delete;

    ImageServer(const ImageServer&) = delete;

    ImageServer& operator=(const ImageServer&) = delete;

    void run();

    void stop();

private:
    void do_accept();

    boost::asio::io_context ioContext;

    boost::asio::ip::tcp::acceptor acceptor;

    ThreadPool threadPool;

    ConnectionPool connectionPool;

    SQLSessionPool sqlSessionPool;
};

#endif //WEBIMG_IMAGESERVER_H
