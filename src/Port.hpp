#pragma once

#include "TypedObject.hpp"

namespace models
{

class Port : public TypedObject 
{
public:
    Port() : TypedObject("", "") {};
    Port(const std::string &name, const std::string &type);
};

}

