#pragma once
#include <string>

namespace models
{

class DocumentedObject
{
    std::string doc;
public:
    DocumentedObject(const std::string &doc);
    const std::string &getDocumentation() const;
};

}
