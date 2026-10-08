#include "product_validation.h"

std::optional<std::string> validate_create(const crow::json::rvalue& body)
{
    for (const std::string& field : {"sku", "name", "price", "description"})
    {
        if (!body.has(field) || body[field].t() != crow::json::type::String)
        {
            return field + " is required and must be a string";
        }
        if (body[field].s().size() == 0)
        {
            return field + " must not be empty";
        }
    }
    if (!body.has("stock_quantity") ||
        body["stock_quantity"].t() != crow::json::type::Number)
    {
        return std::string("stock_quantity is required and must be a number");
    }
    if (body["stock_quantity"].i() < 0)
    {
        return std::string("stock_quantity must not be negative");
    }

    return std::nullopt;  
}
