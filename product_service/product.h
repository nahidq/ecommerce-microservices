#pragma once
#include <string>
struct Product
{
    long long id = 0;
    std::string sku;
    std::string name;
    std::string description;
    std::string price;
    int stock_quantity = 0;
    bool is_active = true;
    std::string created_at;
    std::string updated_at;
};
