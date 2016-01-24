#pragma once

#include "TypedObject.hpp"
#include "DocumentedObject.hpp"
#include <string>
#include <lib_config/Configuration.hpp>
#include <memory>

namespace models
{

class Property : public TypedObject, public DocumentedObject
{
    std::shared_ptr<libConfig::ConfigValue> value;
public:
    Property() : TypedObject("", ""), DocumentedObject("")  {};
    Property(const std::string &name, const std::string &type, const std::string &doc, const std::string &configYML);
    Property(const std::string &name, const std::string &type, const std::string &doc);
    const libConfig::ConfigValue &getValue() const;
    void setValue(std::shared_ptr<libConfig::ConfigValue> newVal);
    bool mergeValue(std::shared_ptr<libConfig::ConfigValue> newVal);
};

}
