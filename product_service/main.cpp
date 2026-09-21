#include <crow.h>
#include <pqxx/pqxx>

#include <cstdlib>
#include <stdexcept>
#include <string>

// Reads one environment variable and fails loudly when it is missing,
// so a misconfigured service dies at startup instead of on first request.
static std::string require_env(const char* name)
{
    const char* value = std::getenv(name);
    if (value == nullptr || *value == '\0')
    {
        throw std::runtime_error(std::string("Missing environment variable: ") + name);
    }
    return std::string(value);
}

static std::string build_connection_string()
{
    return "host=" + require_env("DB_HOST") +
           " port=" + require_env("DB_PORT") +
           " dbname=" + require_env("DB_NAME") +
           " user=" + require_env("DB_USER") +
           " password=" + require_env("DB_PASSWORD");
}

int main()
{
    try
    {
        pqxx::connection connection(build_connection_string());
        CROW_LOG_INFO << "Connected to database " << connection.dbname();

        crow::SimpleApp app;

        CROW_ROUTE(app, "/health")([](){
            crow::json::wvalue response;
            response["status"] = "ok";
            response["service"] = "product_service";
            return response;
        });

        CROW_ROUTE(app, "/products")([&connection]() -> crow::response {
            try
            {
                pqxx::work tx(connection);
                pqxx::result rows = tx.exec(
                    "SELECT id, sku, name, price, stock_quantity "
                    "FROM product_schema.products "
                    "WHERE is_active = true "
                    "ORDER BY id");
                tx.commit();

                crow::json::wvalue::list products;
                for (const auto& row : rows)
                {
                    crow::json::wvalue product;
                    product["id"] = row["id"].as<long long>();
                    product["sku"] = row["sku"].as<std::string>();
                    product["name"] = row["name"].as<std::string>();
                    product["price"] = row["price"].as<std::string>();
                    product["stock_quantity"] = row["stock_quantity"].as<int>();
                    products.push_back(std::move(product));
                }

                crow::json::wvalue response;
                response["products"] = std::move(products);
                return crow::response(response);
            }
            catch (const std::exception& e)
            {
                CROW_LOG_ERROR << "GET /products failed: " << e.what();
                crow::json::wvalue error;
                error["error"] = "database error";
                return crow::response(500, std::move(error));
            }
        });

        // Single threaded on purpose: one pqxx::connection cannot be shared
        // between threads. 
        app.port(8081).run();
    }
    catch (const std::exception& e)
    {
        CROW_LOG_ERROR << "Startup failed: " << e.what();
        return 1;
    }

    return 0;
}
