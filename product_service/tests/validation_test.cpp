#include <gtest/gtest.h>
#include "product_validation.h"

TEST(ValidateCreate, AcceptsValidProduct)
{
    crow::json::rvalue body = crow::json::load(R"({
        "sku": "TSHIRT-BLK-M",
        "name": "Black T-shirt",
        "description": "Cotton, medium",
        "price": "19.99",
        "stock_quantity": 12
        })"
    );
    std::optional<std::string> error = validate_create(body);

    EXPECT_FALSE(error.has_value());
}
TEST(ValidateCreate, RejectsMissingSku)
{
     crow::json::rvalue body = crow::json::load(R"({
        "name": "Black T-shirt",
        "description": "Cotton, medium",
        "price": "19.99",
        "stock_quantity": 12
        })"
    );

    std::optional<std::string> error = validate_create(body);

    EXPECT_TRUE(error.has_value());
    EXPECT_EQ(error.value(), "sku is required and must be a string");
}

TEST(ValidateCreate, InvalidStockQuantity)
{
     crow::json::rvalue body = crow::json::load(R"({
        "sku":"TSHIRT-BLK-M",
        "name": "Black T-shirt",
        "description": "Cotton, medium",
        "price": "19.99",
        "stock_quantity": -1
        })"
    );

    std::optional<std::string> error = validate_create(body);

    EXPECT_TRUE(error.has_value());
    EXPECT_EQ(error.value(), "stock_quantity must not be negative");
}
