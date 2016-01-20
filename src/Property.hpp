#pragma once

#include "TypedObject.hpp"
#include <string>

namespace models
{

class Property : public TypedObject
{
public:
    Property() : TypedObject("", "")  {};
    Property(const std::string &name, const std::string &type);
};

}
