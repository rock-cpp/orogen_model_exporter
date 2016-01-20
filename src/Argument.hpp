#pragma once

#include "TypedObject.hpp"
#include "DocumentedObject.hpp"

namespace models {

class Argument : public TypedObject, public DocumentedObject
{
public:
    Argument(const std::string& name, const std::string& type, const std::string& doc);
};

}

