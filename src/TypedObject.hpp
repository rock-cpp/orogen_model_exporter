#pragma once
#include <string>

namespace models
{

class TypedObject
{
    std::string name;
    std::string type;
public:
    TypedObject(const std::string &name, const std::string &type);
    const std::string &getName() const;
    const std::string &getType() const;
};

}



