#include "YAMLImporter.hpp"
#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <boost/bind.hpp>
#include <boost/function.hpp>

const std::string NAME = "Name";
const std::string DOC = "Doc";
const std::string RETURN = "Return";
const std::string TYPE = "Type";

models::Task models::YAMLImporter::fromFile(const std::string& filename) const
{
    using namespace boost::filesystem;
    
    path path(filename);

    if(!exists(path))
    {
        throw std::runtime_error(std::string("Error, could not find model file ") + path.c_str());
    }

    std::ifstream fin(path.c_str());
    std::stringstream buffer;
    buffer << fin.rdbuf();
    
    return fromString(buffer.str());
}

void fillTaskL1(models::Task &task, const YAML::Node &node);

models::Task models::YAMLImporter::fromString(const std::string& ymlString) const
{
    YAML::Node doc = YAML::Load(ymlString);
    
    models::Task task;
    
    if(doc.Type() != YAML::NodeType::Map)
    {
        std::cout << "Error, model section should only contain yml maps" << std::endl;
//             return false;
    }
    
    if(doc.Type() == YAML::NodeType::Map)
    {
        fillTaskL1(task, doc);
    }
    
    return task;
}

template <class F>
void addTypeWithDoc(const YAML::Node &node, boost::function<void (const F &p)> f)
{
//     std::cout << "Adding " << node[NAME].as<std::string>() << " " <<  node["Type"].as<std::string>() << std::endl;
    f(F(node[NAME].as<std::string>(), node[TYPE].as<std::string>(), node[DOC].as<std::string>()));
}


template <class F>
void addPorts(models::Task &task, const YAML::Node &node, boost::function<void (const F &p)> f)
{
    for(const YAML::Node &portNode: node)
    {
//         std::cout << "Adding " << portNode[NAME].as<std::string>() << " " <<  portNode[TYPE].as<std::string>() << std::endl;
        f(F(portNode[NAME].as<std::string>(), portNode[TYPE].as<std::string>()));
    }
}

void fillTaskL3(models::Task &task, const YAML::Node &node)
{
    addPorts<models::Port>(task, node["inputPorts"], boost::bind(&models::Task::addInputPort, &task, _1));
    addPorts<models::Port>(task, node["outputPorts"], boost::bind(&models::Task::addOutputPort, &task, _1));
    addPorts<models::Property>(task, node["properties"], boost::bind(&models::Task::addProperty, &task, _1));
    
    for(const YAML::Node &op: node["operations"])
    {
        
//         std::cout << "Adding " << portNode[NAME].as<std::string>() << " " <<  portNode[TYPE].as<std::string>() << std::endl;
        std::string opName = op[NAME].as<std::string>();
        std::string ret = op[RETURN][TYPE].as<std::string>();
        std::vector<models::Argument> args;
        
        for(const YAML::Node &arg: op["Arguments"])
        {
            addTypeWithDoc<models::Argument>(arg, [&] (const models::Argument &a) {
                args.push_back(a);
            });
        }
        
        task.addOperation(models::Operation(opName, ret, args));
    }
}

void fillTaskL2(models::Task &task, const YAML::Node &node)
{
    bool first = true;
    for(YAML::const_iterator it = node.begin(); it != node.end(); it++)
    {
        if(!first)
            throw std::runtime_error("models::YAMLImporter::fromString : Error more than one top node detected : second is" + it->first.as<std::string>());
        
        task.setModelName(task.getModelName() + it->first.as<std::string>());
//         std::string memberName = it->first.as<std::string>();
//         std::cout << "Task Name : " << memberName << std::endl;
        
        fillTaskL3(task, it->second);
        first = false;
    }
}

void fillTaskL1(models::Task &task, const YAML::Node &node)
{
    bool first = true;
    for(YAML::const_iterator it = node.begin(); it != node.end(); it++)
    {
        if(!first)
            throw std::runtime_error("models::YAMLImporter::fromString : Error more than one top node detected : second is" + it->first.as<std::string>());
        
//         std::string memberName = it->first.as<std::string>();
//         std::cout << "module Name : " << memberName << std::endl;

        task.setModelName(it->first.as<std::string>() + "::");
        
        fillTaskL2(task, it->second);
        first = false;
    }
}
