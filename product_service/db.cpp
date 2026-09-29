#include "db.h"


static std::string build_connection_string(const Config& config)
{
    return "host=" + config.host +
           " port=" + config.port+
           " dbname=" + config.name +
           " user=" + config.user +
           " password=" + config.password;
}
ConnectionPool::ConnectionPool(const Config& config, std::size_t size)
{
    for (std::size_t i = 0; i< size; ++i)
    {
        connections_.push_back(
            std::make_unique<pqxx::connection>(build_connection_string(config)));

    }
}
PooledConnection  ConnectionPool::acquire()
{
    std::unique_lock<std::mutex> lock(mutex_);
    available_.wait(lock, [this]{return !connections_.empty();});
    std::unique_ptr<pqxx::connection> connection = std::move(connections_.back());
    connections_.pop_back();
    return PooledConnection(*this, std::move(connection));

}
void ConnectionPool::release(std::unique_ptr<pqxx::connection> connection)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        connections_.push_back(std::move(connection));
    }
    available_.notify_one();
}
PooledConnection::PooledConnection(ConnectionPool& pool, 
    std::unique_ptr<pqxx::connection> connection): pool_(pool), connection_(std::move(connection))
{

}
PooledConnection::PooledConnection(PooledConnection&& other) noexcept
 : pool_(other.pool_), connection_(std::move(other.connection_))
{

}
PooledConnection::~PooledConnection()
{
    if (connection_)
    {
        pool_.release(std::move(connection_));
    }
}

pqxx::connection& PooledConnection::get()
{
    return *connection_;
}
