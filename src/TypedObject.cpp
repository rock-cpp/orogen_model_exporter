#include "TypedObject.hpp"

models::TypedObject::TypedObject(const std::string& name, const std::string& type) : name(name), type(type)
{

}

const std::string& models::TypedObject::getName() const
{
    return name;
}

const std::string& models::TypedObject::getType() const
{
    return type;
}
