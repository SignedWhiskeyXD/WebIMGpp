//
// Created by wsmrxd on 2023/7/7.
//

#include "Connection.h"
#include "ConnectionPool.h"
#include "ServletMatcher.h"
#include "ThreadPool.h"

void Connection::handle(){
    std::stringstream ss;
    ss << std::this_thread::get_id();

    switch (connectionState) {
        case Start:
            spdlog::info("Thread {} reading!", ss.str());
            readSocket();
            break;
        case Handle:
            spdlog::info("Thread {} handling!", ss.str());
            doHandle();
            break;
        case Write:
            spdlog::info("Thread {} writing!", ss.str());
            writeSocket();
            break;
        case KeepRead:
            spdlog::info("Thread {} keep reading!", ss.str());
            keepReadSocket();
            break;
        case KeepHandle:
            spdlog::info("Thread {} keep handling!", ss.str());
            keepHandle();
            break;
    }
}

void Connection::disconnect() {
    socket.close();
}

void Connection::readSocket() {
    auto self(shared_from_this());
    socket.async_read_some(boost::asio::buffer(buffer),
        [this, self](boost::system::error_code ec, std::size_t length){
            if(!ec)
            {
                if(length == 0){
                    spdlog::warn("No Content!");
                    return;
                    //readSocket();
                }
                else{
                    connectionState = Handle;
                }
                receiveLength = length;
                queueHandler.commit(self);
            }
            else if (ec != boost::asio::error::operation_aborted)
            {
                spdlog::warn("Close on Reading!");
                poolHandler.closeConnection(shared_from_this());
            }
        });
}

void Connection::doHandle() {
    auto self(shared_from_this());

    std::string_view view(buffer.begin(), buffer.begin() + receiveLength);
    std::stringstream ss(view.data());
    request = RequestParser::parseRequest(ss).second;

    auto payloadPos = view.find("\r\n\r\n");
    if(payloadPos != std::string::npos && payloadPos + 4 < receiveLength){
        request.payload = std::string(buffer.begin() + payloadPos + 4,
                                      buffer.begin() + receiveLength);
    }

    if(receiveLength == 8192)
        connectionState = KeepRead;
    else {
        auto servlet = ServletMatcher::getInstance().match(request);
        response = servlet->onHandle(request);
        connectionState = Write;
    }
    queueHandler.commit(self);
}

void Connection::writeSocket() {
    auto self(shared_from_this());
    boost::asio::async_write(socket, response.toAsioBuffers(),
        [this, self](boost::system::error_code ec, std::size_t length){
            if (!ec)
            {
                if(request.headers.contains("Connection") &&
                   request.headers["Connection"] == "keep-alive")
                {
                    request = HTTPRequest();
                    response = HTTPResponse();
                    connectionState = Start;
                    queueHandler.commit(self);
                }
                else{
                    spdlog::warn("Not alive connection, closing");
                    boost::system::error_code ignored_ec;
                    self->socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                          ignored_ec);
                    poolHandler.closeConnection(shared_from_this());
                }
            }
            if (ec && ec != boost::asio::error::operation_aborted)
            {
                spdlog::warn("Close on writing error: {}", ec.what());
                poolHandler.closeConnection(shared_from_this());
            }
        });
}

void Connection::keepReadSocket() {
    auto self(shared_from_this());
    socket.async_read_some(boost::asio::buffer(buffer),
        [this, self](boost::system::error_code ec, std::size_t length){
           if(!ec)
           {
               connectionState = KeepHandle;
               receiveLength = length;
               queueHandler.commit(self);
           }
           else if (ec != boost::asio::error::operation_aborted)
           {
               spdlog::warn("Close on keep Reading!");
               poolHandler.closeConnection(shared_from_this());
           }
        });
}

void Connection::keepHandle() {
    auto self(shared_from_this());

    request.payload += std::string(buffer.begin(), buffer.begin() + receiveLength);

    if(receiveLength == 8192)
        connectionState = KeepRead;
    else {
        auto servlet = ServletMatcher::getInstance().match(request);
        response = servlet->onHandle(request);
        connectionState = Write;
    }
    queueHandler.commit(self);
}