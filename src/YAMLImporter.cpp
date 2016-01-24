#include "YAMLImporter.hpp"
#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace models {
    

const std::string NAME = "Name";
const std::string DOC = "Doc";
const std::string RETURN = "Return";
const std::string TYPE = "Type";
const std::string DEFAULT_VALUE = "DefaultVal";

Task YAMLImporter::fromFile(const std::string& filename) const
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

Task YAMLImporter::fromString(const std::string& ymlString) const
{
    YAML::Node doc = YAML::Load(ymlString);
    
    Task task;
    
    if(doc.Type() != YAML::NodeType::Map)
    {
        std::cout << "Error, model section should only contain yml maps" << std::endl;
//             return false;
    }
    
    if(doc.Type() == YAML::NodeType::Map)
    {
        bool onlyOuter = true;
        bool onlyInner = true;
        for(const auto &outer: doc)
        {
            if(!onlyOuter)
                throw std::runtime_error("YAMLImporter::fromString : Error more than one top node detected : second is" + outer.first.as<std::string>());
            
//             std::cout << "IS Sequence" << 
            task.setModuleName(outer.first.as<std::string>());
            
            for(const auto &inner: outer.second)
            {
                if(!onlyInner)
                    throw std::runtime_error("YAMLImporter::fromString : Error more than one top node detected : second is" + inner.first.as<std::string>());

                task.setTaskName(inner.first.as<std::string>());
            
                fillTask(task, inner.second);
                
                onlyInner = false;
            }        
            onlyOuter = false;
        }
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
void addPorts(const YAML::Node &node, boost::function<void (const F &p)> f)
{
    for(const YAML::Node &portNode: node)
    {
//         std::cout << "Adding " << portNode[NAME].as<std::string>() << " " <<  portNode[TYPE].as<std::string>() << std::endl;
        f(F(portNode[NAME].as<std::string>(), portNode[TYPE].as<std::string>()));
    }
}

void addProperty(const YAML::Node &node, boost::function<void (const Property &p)> f)
{
    std::string doc;
    
    if(node[DOC])
        doc = node[DOC].as<std::string>();
    
    if(node[DEFAULT_VALUE])
    {
        f(Property(node[NAME].as<std::string>(), node[TYPE].as<std::string>(), doc, node[DEFAULT_VALUE].as<std::string>()));
    }
    else
    {
        f(Property(node[NAME].as<std::string>(), node[TYPE].as<std::string>(), doc));
    }
}


void addProperties(const YAML::Node &node, boost::function<void (const Property &p)> f)
{
    for(const YAML::Node &propNode: node)
    {
        addProperty(propNode, f);
    }    
}

void YAMLImporter::addPlugins(RuntimeModel& model, const YAML::Node& node) const
{
    if(node["transformer"])
    {
        std::cout << "The Transformer is ehre " << std::endl;
        TransformerPlugin *trPlugin = dynamic_cast<TransformerPlugin *>(PluginStore::getInstace().getNewPluginInstance("transformer"));
        
        if(!trPlugin)
            throw std::runtime_error("Error, Transformer Plugin not registered");
        
        model.registerPlugin(trPlugin);
        
        const YAML::Node &tr(node["transformer"]);
        
        for(const YAML::Node &frame : tr["Frames"])
        {
            trPlugin->frames.push_back(frame.as<std::string>());
        }
        
        for(const YAML::Node &trans : tr["Transformations"])
        {
            trPlugin->unMappedTransformations.push_back(Transformation(trans["From"].as<std::string>(), trans["To"].as<std::string>()));
        }
    }
}

void YAMLImporter::fillTask(Task& task, const YAML::Node& input) const
{
    bool onlyOuter = true;
    bool onlyInner = true;
    
    const YAML::Node *nodeP;
    
    for(const auto &outer: input)
    {
        if(!onlyOuter)
            throw std::runtime_error("YAMLImporter::fromString : Error more than one top node detected : second is" + outer.first.as<std::string>());
        
        task.setModuleName(outer.first.as<std::string>());
            
        for(const auto &inner: outer.second)
        {
            if(!onlyInner)
                throw std::runtime_error("YAMLImporter::fromString : Error more than one top node detected : second is" + inner.first.as<std::string>());

            task.setTaskName(inner.first.as<std::string>());
        
            nodeP = &(inner.second);
            
            onlyInner = false;
        }        
        onlyOuter = false;
    }

    const YAML::Node &node(*nodeP);
    
    addPorts<Port>(node["inputPorts"], boost::bind(&Task::addInputPort, &task, _1));
    addPorts<Port>(node["outputPorts"], boost::bind(&Task::addOutputPort, &task, _1));
    addProperties(node["properties"], boost::bind(&Task::addProperty, &task, _1));
    
    for(const YAML::Node &op: node["operations"])
    {
        
//         std::cout << "Adding " << portNode[NAME].as<std::string>() << " " <<  portNode[TYPE].as<std::string>() << std::endl;
        std::string opName = op[NAME].as<std::string>();
        std::string ret = op[RETURN][TYPE].as<std::string>();
        std::vector<Argument> args;
        
        for(const YAML::Node &arg: op["Arguments"])
        {
            addTypeWithDoc<Argument>(arg, [&] (const Argument &a) {
                args.push_back(a);
            });
        }
        
        task.addOperation(Operation(opName, ret, args));
    }
}

RuntimeModel YAMLImporter::getRunntimeModelfromFile(const std::string& filename) const
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
    
    return getRunntimeModelFromString(buffer.str());    
}

RuntimeModel YAMLImporter::getRunntimeModelFromString(const std::string& ymlString) const
{
    YAML::Node doc = YAML::Load(ymlString);
    

    if(doc.Type() != YAML::NodeType::Map)
    {
        std::cout << "Error, model section should only contain yml maps" << std::endl;
    }

    Task task;
    
    fillTask(task, doc);

    RuntimeModel rModel(task);

    if(doc[task.getModuleName()][task.getTaskName()]["Plugins"])
    {
        std::cout << "Got Plugins " << std::endl;
        addPlugins(rModel, doc[task.getModuleName()][task.getTaskName()]["Plugins"]);
    }
    
    return rModel;
    
}

}