#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <unordered_set>
#include <spdlog/spdlog.h>
#include "Connection.h"

class ConnectionPool{
public:
    ConnectionPool() = default;

    ConnectionPool(const ConnectionPool&) = delete;

    ConnectionPool& operator=(const ConnectionPool&) = delete;
     
    bool addConnection(ConnectionPtr newConnection){
        spdlog::info("New Connection!");
        return aliveConnections.insert(newConnection).second;
        spdlog::info("num: {}", aliveConnections.size());
    }

    bool closeConnection(ConnectionPtr connection){
        spdlog::warn("Connection disconnected!");
        auto numsRemoved = aliveConnections.erase(connection);
        return static_cast<bool>(numsRemoved);
        spdlog::info("num: {}", aliveConnections.size());
    }

    void closeAll(){
        aliveConnections.clear();
    }

    std::size_t size() const{
        return aliveConnections.size();
    }

private:
    std::unordered_set<ConnectionPtr> aliveConnections;
};

#endif