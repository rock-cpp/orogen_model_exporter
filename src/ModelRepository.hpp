#pragma once

#include "Task.hpp"

namespace models
{

class ModelRepository
{
public:
    
    void registerSpecializedModel(Task model);
    
    bool getModel(const std::string &modelName, Task &model);
};

}
