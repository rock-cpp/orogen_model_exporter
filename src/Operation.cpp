#include "Operation.hpp"

models::Operation::Operation(const std::string& name, const std::string& ret, const std::vector< models::Argument >& arguments) : name(name), returnType(ret), arguments(arguments)
{

}

const std::vector< models::Argument >& models::Operation::getArguments() const
{
    return arguments;
}

const std::string& models::Operation::getName() const
{
    return name;
}

const std::string& models::Operation::getReturnType() const
{
    return returnType;
}
