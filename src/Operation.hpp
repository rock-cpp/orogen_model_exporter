#pragma once

#include "Argument.hpp"
#include <vector>

namespace models {

class Operation
{
    std::string name;
    std::string returnType;
    std::vector<Argument> arguments;
public:
    Operation() {};
    Operation(const std::string &name, const std::string &retType, const std::vector<Argument> &arguments);
    const std::string &getName() const;
    const std::string &getReturnType() const;
    const std::vector<Argument> &getArguments() const;
};

}
