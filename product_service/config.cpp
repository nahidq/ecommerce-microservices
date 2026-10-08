#include "config.h"
#include <stdexcept>


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

Config load_config()
{
    Config config;
    config.host = require_env("DB_HOST");
    config.port = require_env("DB_PORT");
    config.name = require_env("DB_NAME");
    config.user = require_env("DB_USER");
    config.password = require_env("DB_PASSWORD");
    return config;
}
