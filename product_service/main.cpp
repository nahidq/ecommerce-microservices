#include <crow.h>
#include <stdexcept>
#include <string>
#include "config.h"
#include "product_routes.h"
#include "db.h"


int main()
{
    try
    {
        Config config = load_config();
        ConnectionPool pool(config,8);
        ProductRepository repository(pool);
        crow::SimpleApp app;
        register_product_routes(app, repository);
        register_health_routes(app);
        app.port(8081).multithreaded().run();

    }
    catch(const std::exception& e)
    {
       CROW_LOG_ERROR << "Startup failed: " << e.what();
       return 1;
    }
    return 0;
}
