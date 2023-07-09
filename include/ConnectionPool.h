#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <unordered_set>
#include "Connection.h"

class ConnectionPool{
public:
    ConnectionPool() = default;

    ConnectionPool(const ConnectionPool&) = delete;

    ConnectionPool& operator=(const ConnectionPool&) = delete;
     
    bool addConnection(ConnectionPtr newConnection){
        return aliveConnections.insert(newConnection).second;
    }

    bool closeConnection(ConnectionPtr connection){
        auto numsRemoved = aliveConnections.erase(connection);
        return static_cast<bool>(numsRemoved);
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