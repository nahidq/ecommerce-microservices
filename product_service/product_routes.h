#pragma once

#include<crow.h>

#include "product_repository.h"

void register_product_routes(crow::SimpleApp& app, ProductRepository& repository);

void register_health_routes(crow::SimpleApp& app);
