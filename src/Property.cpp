#include "Property.hpp"
#include <lib_config/YAMLConfiguration.hpp>
#include <iostream>

models::Property::Property(const std::string& name, const std::string& type, const std::string& doc, const std::string& configYML): TypedObject(name, type), DocumentedObject(doc)
{
    libConfig::YAMLConfigParser parser;
    value = parser.getConfigValue(configYML);
    
    value->setName(name);
}

models::Property::Property(const std::string& name, const std::string& type, const std::string& doc): TypedObject(name, type), DocumentedObject(doc)
{

}

// models::Property::Property(const std::string& name, const std::string& type): TypedObject(name, type), value(nullptr)
// {
// 
// }

void models::Property::setValue(std::shared_ptr<libConfig::ConfigValue> newVal)
{
    
    if(!value)
    {
        switch(newVal->getType())
        {
            case libConfig::ConfigValue::ARRAY:
                value.reset(new libConfig::ArrayConfigValue());
                break;
            case libConfig::ConfigValue::COMPLEX:
                value.reset(new libConfig::ComplexConfigValue());
                break;
            case libConfig::ConfigValue::SIMPLE:
                value.reset(new libConfig::SimpleConfigValue(""));
                break;
                
        }
        value->setName(getName());
    }
    
    if(value->getType() != newVal->getType())
        throw std::runtime_error("Error, configuration values may not change !");

    value = newVal;
}

const libConfig::ConfigValue& models::Property::getValue() const
{
    return *value;
}

bool models::Property::mergeValue(std::shared_ptr< libConfig::ConfigValue > newVal)
{
    if(!value)
    {
        setValue(newVal);
        return true;
    }
    
    if(value->getType() != newVal->getType())
        throw std::runtime_error("Error, configuration values may not change !");

    return value->merge(newVal);
}
