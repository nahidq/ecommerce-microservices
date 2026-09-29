#pragma once
#include <pqxx/pqxx>
#include <string>
#include "config.h"
#include <cstddef>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>

class PooledConnection;
class ConnectionPool
{
public:
    ConnectionPool(const Config& config, std::size_t size);
    PooledConnection acquire();
    void release(std::unique_ptr<pqxx::connection> connection);

private:
    std::mutex mutex_;
    std::condition_variable available_;
    std::vector<std::unique_ptr<pqxx::connection>> connections_;
};

class PooledConnection
{
public:
    PooledConnection(ConnectionPool& pool, std::unique_ptr<pqxx::connection> connection);
    ~PooledConnection();
    PooledConnection(const PooledConnection&) = delete;
    PooledConnection& operator=(const PooledConnection&) = delete;
    PooledConnection(PooledConnection&& other) noexcept;
    pqxx::connection& get();

private:
    ConnectionPool& pool_;
    std::unique_ptr<pqxx::connection> connection_;

};
