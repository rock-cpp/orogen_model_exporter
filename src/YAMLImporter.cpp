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
        bool onlyOuter = true;
        bool onlyInner = true;
        for(const auto &outer: doc)
        {
            if(!onlyOuter)
                throw std::runtime_error("models::YAMLImporter::fromString : Error more than one top node detected : second is" + outer.first.as<std::string>());
            
//             std::cout << "IS Sequence" << 
            
            for(const auto &inner: outer.second)
            {
                if(!onlyInner)
                    throw std::runtime_error("models::YAMLImporter::fromString : Error more than one top node detected : second is" + inner.first.as<std::string>());

                task.setModelName(outer.first.as<std::string>() + "::" + task.getModelName() + inner.first.as<std::string>());
            
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
void addPorts(models::Task &task, const YAML::Node &node, boost::function<void (const F &p)> f)
{
    for(const YAML::Node &portNode: node)
    {
//         std::cout << "Adding " << portNode[NAME].as<std::string>() << " " <<  portNode[TYPE].as<std::string>() << std::endl;
        f(F(portNode[NAME].as<std::string>(), portNode[TYPE].as<std::string>()));
    }
}


void models::YAMLImporter::addPlugins(models::RuntimeModel& model, const YAML::Node& node) const
{
    if(node["transformer"])
    {
        TransformerPlugin *trPlugin = dynamic_cast<TransformerPlugin *>(PluginStore::getInstace().getNewPluginInstance("transformer"));
        model.registerPlugin(trPlugin);
        
        const YAML::Node &tr(node["transformer"]);
        
        for(const YAML::Node &frame : tr["Frames"])
        {
//             task.frames.push_back(frame.as<std::string>());
        }
        
        for(const YAML::Node &trans : tr["Transformations"])
        {
//             task.transformations.push_back(Transformation(trans["From"].as<std::string>(), trans["To"].as<std::string>()));
        }
        
//     for(const YAML::Node &op: )
//     {
//         
// //         std::cout << "Adding " << portNode[NAME].as<std::string>() << " " <<  portNode[TYPE].as<std::string>() << std::endl;
//         std::string opName = op[NAME].as<std::string>();
//         std::string ret = op[RETURN][TYPE].as<std::string>();
//         std::vector<models::Argument> args;
//         
//         for(const YAML::Node &arg: op["Arguments"])
//         {
//             addTypeWithDoc<models::Argument>(arg, [&] (const models::Argument &a) {
//                 args.push_back(a);
//             });
//         }
//         
//         task.addOperation(models::Operation(opName, ret, args));
//     }

    }
}

void models::YAMLImporter::fillTask(models::Task& task, const YAML::Node& input) const
{
    bool onlyOuter = true;
    bool onlyInner = true;
    
    const YAML::Node *nodeP;
    
    for(const auto &outer: input)
    {
        if(!onlyOuter)
            throw std::runtime_error("models::YAMLImporter::fromString : Error more than one top node detected : second is" + outer.first.as<std::string>());
        
//             std::cout << "IS Sequence" << 
        
        for(const auto &inner: outer.second)
        {
            if(!onlyInner)
                throw std::runtime_error("models::YAMLImporter::fromString : Error more than one top node detected : second is" + inner.first.as<std::string>());

            task.setModelName(outer.first.as<std::string>() + "::" + task.getModelName() + inner.first.as<std::string>());
        
            nodeP = &(inner.second);
            
            onlyInner = false;
        }        
        onlyOuter = false;
    }

    const YAML::Node &node(*nodeP);
    
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

//     if(node["Plugins"])
//     {
//         addPlugins(task, node["Plugins"]);
//     }
//     
}

models::RuntimeModel models::YAMLImporter::getRunntimeModelfromFile(const std::string& filename) const
{
    
}

models::RuntimeModel models::YAMLImporter::getRunntimeModelFromString(const std::string& ymlString) const
{
    YAML::Node doc = YAML::Load(ymlString);
    

    if(doc.Type() != YAML::NodeType::Map)
    {
        std::cout << "Error, model section should only contain yml maps" << std::endl;
//             return false;
    }


    Task task;
    
    fillTask(task, doc);

    models::RuntimeModel rModel(task);

    
    if(doc[0][0]["Plugins"])
    {
        addPlugins(rModel, doc[0][0]["Plugins"]);
    }
    
    return rModel;
    
}

