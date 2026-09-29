#pragma once
#include <string>

struct Config
{
    std::string host;
    std::string port;
    std::string name;
    std::string user;
    std::string password; 
};

Config load_config();
