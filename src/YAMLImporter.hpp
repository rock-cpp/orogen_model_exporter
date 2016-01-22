#pragma once

#include "Task.hpp"
#include "RuntimeModel.hpp"

namespace YAML 
{
class Node;
}

namespace models
{

class YAMLImporter
{
    void fillTask(models::Task &task, const YAML::Node &node) const;
    void addPlugins(models::RuntimeModel &model, const YAML::Node &node) const;

public:
    Task fromFile(const std::string &filename) const;
    Task fromString(const std::string &ymlString) const;
    RuntimeModel getRunntimeModelfromFile(const std::string &filename) const;
    RuntimeModel getRunntimeModelFromString(const std::string &ymlString) const;
};

}
