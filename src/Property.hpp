#pragma once

#include "TypedObject.hpp"
#include <string>

namespace models
{

class Property : public TypedObject
{
    std::string value;
public:
    Property() : TypedObject("", "")  {};
    Property(const std::string &name, const std::string &type);
    const std::string getValue() const;
    void setValue(const std::string &newVal);
};

}
