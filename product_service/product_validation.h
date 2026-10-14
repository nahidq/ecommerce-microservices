#pragma once

#include <crow.h>
#include <optional>
#include <string>


std::optional<std::string> validate_create(const crow::json::rvalue& body);
