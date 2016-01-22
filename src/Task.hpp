#pragma once

#include <vector>
#include <map>
#include "Port.hpp"
#include "Property.hpp"
#include "Operation.hpp"
#include <iostream>

namespace models
{

class YAMLImporter;
    
class Transformation
{
    std::string source;
    std::string target;
public:
    Transformation(std::string source, std::string target) : source(source), target(target) {};
    const std::string &getSourceFrame() const;
    const std::string &getTargetFrame() const;
    
};
    
class Task
{
friend class models::YAMLImporter;
    std::vector<Port> inputPorts;
    std::vector<Port> outputPorts;
    std::vector<Property> properties;
    std::vector<Operation> operations;
    
    std::map<std::string, Port> inputPortMap;
    std::map<std::string, Port> outputPortMap;
    std::map<std::string, Property> propertyMap;
    std::map<std::string, Operation> operationMap;
    
    std::string moduleName;
    std::string taskName;
    
public:
    
    Task();
    void setModuleName(const std::string &name);
    const std::string &getModuleName() const;

    void setTaskName(const std::string &name);
    const std::string &getTaskName() const;
    
    const std::string getModelName() const;
    
    void addInputPort(const Port &port);
    void addOutputPort(const Port &port);
    void addProperty(const Property &property);
    void addOperation(const Operation &op);
    
    const std::vector<Port> &getInputPorts() const; 
    const std::vector<Port> &getOutputPorts() const; 

    bool hasInputPort(const std::string name) const;
    const Port &getInputPort(const std::string name) const;
    
    bool hasOutputPort(const std::string name) const;
    const Port &getOutputPort(const std::string name) const;
    
    const std::vector<Property> &getProperties() const; 

    bool hasProperty(const std::string name) const;
    const Property &getProperty(const std::string name) const;
    
    bool hasOperation(const std::string name) const;
    const Operation &getOperation(const std::string name) const;
    const std::vector<Operation> &getOperations() const; 
    
    friend std::ostream& operator<< (std::ostream& s, const Task& t) 
    {
        s << "Task Model Name : " << t.getModelName() << std::endl;
        s << "InputPorts : " << std::endl;
        for(const Port &p: t.getInputPorts())
        {
            s << "    " << p.getName() << "<" << p.getType() << ">" << std::endl;
        }        
        s << "OutputPorts : " << std::endl;
        for(const Port &p: t.getOutputPorts())
        {
            s << "    " << p.getName() << "<" << p.getType() << ">" << std::endl;
        }        
        s << "Properties : " << std::endl;
        for(const Property &p: t.getProperties())
        {
            s << "    " << p.getName() << "<" << p.getType() << ">" << std::endl;
        }
        s << "Operations : " << std::endl;
        for(const Operation &p: t.getOperations())
        {
            s << "    " << p.getReturnType() << " " << p.getName() << "(";
            bool first = true;
            for(const Argument &a: p.getArguments())
            {
                if(!first)
                    s << ", ";

                s << a.getType() << " " << a.getName();
                
                
                first = false;
            }
            s << ")" << std::endl; 
        }

//         if(t.hasTransformer())
//         {
//             s << "Known Frames : " << std::endl;
//             for(const std::string &p: t.getTransformerFrames())
//             {
//                 s << "    " << p << std::endl;
//             }
//             s << "Transformations : " << std::endl;
//             for(const Transformation &p: t.getUnmappedTransformations())
//             {
//                 s << "    " << p.getSourceFrame() << "2" << p.getTargetFrame() << std::endl;
//             }
//         }
        
        return s;
    }
};

}
