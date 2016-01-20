#pragma once

#include "Task.hpp"

namespace models
{

class YAMLImporter
{
public:
    Task fromFile(const std::string &filename) const;
    Task fromString(const std::string &ymlString) const;
};

}
