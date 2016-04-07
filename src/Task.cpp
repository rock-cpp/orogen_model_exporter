#include "Task.hpp"
#include <stdexcept>

const std::string& models::Transformation::getSourceFrame() const
{
    return source;
}

const std::string& models::Transformation::getTargetFrame() const
{
    return target;
}


models::Task::Task()
{

}

void models::Task::setModuleName(const std::string& name)
{
    moduleName = name;
}

const std::string& models::Task::getModuleName() const
{
    return moduleName;
}

void models::Task::setTaskName(const std::string& name)
{
    taskName = name;
}

const std::string& models::Task::getTaskName() const
{
    return taskName;
}

const std::string models::Task::getModelName() const
{
    return moduleName + "::" + taskName;
}


template <class F>
void addObject(std::map<std::string, F> &map, std::vector<F> &vec, const F& o, const std::string &objectName)
{
    if(map.find(o.getName()) != map.end())
        throw std::runtime_error("models::Task::add" + objectName +" : Error, there is already a " + objectName + " with the name " + o.getName() + " registered ");
    
    map[o.getName()] = o;
    vec.push_back(o);
}

void models::Task::addInputPort(const models::Port& port)
{
    addObject(inputPortMap, inputPorts, port, "InputPort");
}

void models::Task::addOutputPort(const models::Port& port)
{
    addObject(outputPortMap, outputPorts, port, "OutputPort");
}

void models::Task::addProperty(const models::Property& property)
{
    addObject(propertyMap, properties, property, "Property");
}

void models::Task::addOperation(const models::Operation& op)
{
    addObject(operationMap, operations, op, "Operation");
}

template <class F>
bool hasValue(const std::map<std::string, F> &map, const std::string& name)
{
    return map.find(name) != map.end();
}

template <class F>
const F &getObject(const std::map<std::string, F> &map, const std::string& name, const std::string &objectName)
{
    typename std::map<std::string, F>::const_iterator it = map.find(name);
    if(it == map.end())
    {
        throw std::runtime_error("models::Task::get" + objectName + " : Error no " + objectName + " with name " + name + " registered");
    }
    
    return it->second;
}

template <class F>
F &getObjectNC(std::map<std::string, F> &map, const std::string& name, const std::string &objectName)
{
    typename std::map<std::string, F>::iterator it = map.find(name);
    if(it == map.end())
    {
        throw std::runtime_error("models::Task::get" + objectName + " : Error no " + objectName + " with name " + name + " registered");
    }
    
    return it->second;
}

bool models::Task::hasInputPort(const std::string name) const
{
    return hasValue(inputPortMap, name);
}

const models::Port& models::Task::getInputPort(const std::string name) const
{
    return getObject(inputPortMap, name, "InputPort");
}

bool models::Task::hasOutputPort(const std::string name) const
{
    return hasValue(outputPortMap, name);
}

const models::Port& models::Task::getOutputPort(const std::string name) const
{
    return getObject(outputPortMap, name, "OutputPort");
}

bool models::Task::hasProperty(const std::string name) const
{
    return hasValue(propertyMap, name);
}

const models::Property& models::Task::getProperty(const std::string name) const
{
    return getObject(propertyMap, name, "Property");
}

models::Property& models::Task::getProperty(const std::string name)
{
    return getObjectNC(propertyMap, name, "Property");
}

bool models::Task::hasOperation(const std::string name) const
{
    return hasValue(operationMap, name);
}

const models::Operation& models::Task::getOperation(const std::string name) const
{
    return getObject(operationMap, name, "Operation");
}

const std::vector< models::Port >& models::Task::getInputPorts() const
{
    return inputPorts;
}

const std::vector< models::Port >& models::Task::getOutputPorts() const
{
    return outputPorts;
}

const std::vector< models::Property >& models::Task::getProperties() const
{
    return properties;
}

const std::vector< models::Operation >& models::Task::getOperations() const
{
    return operations;
}



