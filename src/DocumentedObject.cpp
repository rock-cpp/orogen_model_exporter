#include "DocumentedObject.hpp"

models::DocumentedObject::DocumentedObject(const std::string& doc) : doc(doc)
{

}

const std::string& models::DocumentedObject::getDocumentation() const
{
    return doc;
}
