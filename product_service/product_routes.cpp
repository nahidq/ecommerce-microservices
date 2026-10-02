#include "product_routes.h"

static crow::json::wvalue product_to_json(const Product& item)
{
        crow::json::wvalue product;
        product["id"] = item.id;
        product["sku"] = item.sku;
        product["name"] = item.name;
        product["price"] = item.price;
        product["stock_quantity"] = item.stock_quantity;
        product["description"] = item.description;

        return product;
}
static crow::response list_products(ProductRepository& repository)
{
     try
        {
            crow::json::wvalue::list products;
            auto allproducts = repository.get_products();
            for(const auto& item: allproducts)
            {
                products.push_back(product_to_json(item));
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
        
}

static crow::response get_product(ProductRepository& repository, long long id)
{
    try
    {
       crow::json::wvalue response;
        auto product = repository.get_product_by_id(id);
        if (!product)
        {
            response["error"] = "Item not found";
            return crow::response(404, response);
        }
        
        response["product"] = product_to_json(*product);
        return crow::response(response);
    }
    catch(const std::exception& e)
    {
        CROW_LOG_ERROR << "GET /product/id failed: " << e.what();
        crow::json::wvalue error;
        error["error"] = "database error";
        return crow::response(500, std::move(error));
    }
     
}

void register_product_routes(crow::SimpleApp& app, ProductRepository& repository)
{
    CROW_ROUTE(app, "/products")([&repository]()-> crow::response
    {
        return list_products(repository);
    });
    CROW_ROUTE(app, "/products/<int>")([&repository](long long id){
        return get_product(repository, id);
    });
}

void register_health_routes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/health")([](){
            crow::json::wvalue response;
            response["status"] = "ok";
            response["service"] = "product_service";
            return response;
        });   
}
