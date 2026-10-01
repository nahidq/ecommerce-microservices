
#include "product_repository.h"
#include  "db.h"


ProductRepository::ProductRepository(ConnectionPool& pool):pool_(pool)
{

}

static Product read_product(const pqxx::row& row)
{
    Product product;
    product.id = row["id"].as<long long>();
    product.sku = row["sku"].as<std::string>();
    product.name = row["name"].as<std::string>();
    product.description = row["description"].as<std::string>();
    product.price = row["price"].as<std::string>();
    product.stock_quantity = row["stock_quantity"].as<int>();
    return product;
}

std::vector<Product> ProductRepository::get_products()
{
    std::vector <Product> products;
  
    PooledConnection conn = pool_.acquire();
    pqxx::work tx(conn.get());
    pqxx::result rows = tx.exec(
        "SELECT id, sku, name, price, stock_quantity, description "
                "FROM product_schema.products "
                "WHERE is_active = true "
                "ORDER BY id");
    tx.commit();

    for(const auto& row : rows)
    {
        products.push_back(read_product(row)); 

    }
    return products;
}
std::optional<Product> ProductRepository::get_product_by_id(long long id)
{
    PooledConnection conn = pool_.acquire();
    pqxx::work tx(conn.get());
    pqxx::result rows = tx.exec_params(
        "SELECT id, sku, name, description, price, stock_quantity "
        "FROM product_schema.products "
        "WHERE id = $1 AND is_active = true",
    id);
    tx.commit();
    if (rows.empty())
    {
         return std::nullopt;
    }
    return read_product(rows[0]);
}
