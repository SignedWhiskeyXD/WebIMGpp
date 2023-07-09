//
// Created by wsmrxd on 2023/7/7.
//

#include "Connection.h"
#include "ConnectionPool.h"
#include "ServletMatcher.h"

void Connection::handle(){
    readSocket();
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
                    readSocket();
                }
                else{
                    std::string_view view(buffer.begin(), buffer.begin() + length);
                    std::stringstream ss(view.data());
                    request = RequestParser::parseRequest(ss).second;

                    // TODO: 已经解析好请求，响应之
                    auto servlet = ServletMatcher::getInstance().match(request);
                    if(servlet != nullptr)
                        response = servlet->onHandle(request);
                    else {
                        response = ResponseBuilder()
                                    .setStatus(400, "Bad Request")
                                    .setPayload("WTF R U DOING").build();
                    }
                    writeSocket();
                }
            }
            else if (ec != boost::asio::error::operation_aborted)
            {
                poolHandler.closeConnection(shared_from_this());
            }
        });
}

void Connection::writeSocket() {
    auto self(shared_from_this());
    boost::asio::async_write(socket, response.toAsioBuffers(),
        [this, self](boost::system::error_code ec, std::size_t length){
            if (!ec)
            {
                boost::system::error_code ignored_ec;
                self->socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                    ignored_ec);
            }
            if (ec != boost::asio::error::operation_aborted)
            {
                poolHandler.closeConnection(shared_from_this());
            }
        });
}
