#pragma once
#include <optional>
#include <vector>

#include "db.h"
#include "product.h"

class ProductRepository
{
public:
    explicit ProductRepository(ConnectionPool& pool);
    std::vector<Product> get_products();
    std::optional<Product> get_product_by_id(long long id);
    Product create(const Product& product);
private:
    ConnectionPool& pool_;

};
