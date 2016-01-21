#pragma once

#include "Task.hpp"

namespace YAML 
{
class Node;
}

namespace models
{

class YAMLImporter
{
    void fillTask(models::Task &task, const YAML::Node &node) const;

public:
    Task fromFile(const std::string &filename) const;
    Task fromString(const std::string &ymlString) const;
};

}
