#include "RuntimeModel.hpp"

models::PluginStore *models::PluginStore::instance = nullptr;

models::PluginStore::~PluginStore()
{
    for(auto it : plugins)
        delete it.second;
    
    plugins.clear();
}


models::PluginStore& models::PluginStore::getInstace()
{
    if(!instance)
        instance = new PluginStore();
    
    return *instance;
}


models::RuntimePlugin* models::PluginStore::getNewPluginInstance(const std::string& name)
{
    auto it = plugins.find(name);
    if(it == plugins.end())
        return nullptr;
    
    return it->second->getNewInstance();
}

models::PluginStore::PluginStore()
{

}

void models::PluginStore::registerPlugin(models::RuntimePlugin* plugin)
{
    auto it = plugins.find(plugin->getName());
    if(it != plugins.end())
        throw std::runtime_error("There is already a plugin with the name " + plugin->getName() + " registered");
    
    plugins[plugin->getName()] = plugin;
}


const models::Task& models::RuntimeModel::getCurrentTaskState()
{
    return taskState;
}

void models::RuntimeModel::registerPlugin(models::RuntimePlugin* plugin)
{
    auto it = plugins.find(plugin->getName());
    if(it != plugins.end())
        throw std::runtime_error("There is already a plugin with the name " + plugin->getName() + " registered");
    
    plugin->setTaskModel(taskState);
    
    plugins[plugin->getName()] = plugin;
}

models::RuntimePlugin* models::RuntimeModel::getPlugin(const std::string& name)
{
    auto it = plugins.find(name);
    if(it == plugins.end())
        return nullptr;
    
    return it->second;
}

models::TransformerPlugin::TransformerPlugin(): RuntimePlugin("transformer")
{

}

void models::RuntimePlugin::applyConfig(const libConfig::Configuration &config)
{
    for(const auto &cv: config.getValues())
    {
        if(!getModel().hasProperty(cv.first))
            throw std::runtime_error("Error, task " + getName() + " of type " + getModel().getModelName() + " has not propertie " + cv.first);
        
        Property &p(getModel().getProperty(cv.first));
        
        p.mergeValue(cv.second);
    }
    
}

bool models::TransformerPlugin::configure()
{
    std::map<std::string, std::string> frameRemapMap;
    
    for(const std::string &frame : frames)
    {
        const std::string propName = frame + "_frame";
        if(!getModel().hasProperty(propName))
            throw std::runtime_error("Internal Error, task model does not contain the expected frame renaming properties");
        
        const Property &prop(getModel().getProperty(propName));
        
        if(prop.getValue().getType() != libConfig::ConfigValue::SIMPLE)
        {
            throw std::runtime_error("Internal Error, remap properties must be SIMPLE config values (aka string)");
        }
        
        const libConfig::SimpleConfigValue *sval = dynamic_cast<const libConfig::SimpleConfigValue *>(&(prop.getValue()));
        
        frameRemapMap[frame] = sval->getValue();
        std::cout << "Config Value  is ";
        sval->print(std::cout);
        std::cout << std::endl;;
    }

    transformations.clear();
    transformations.reserve(unMappedTransformations.size());
    
    for(const Transformation &tr : unMappedTransformations)
    {
        transformations.push_back(Transformation(frameRemapMap[tr.getSourceFrame()], 
                                                frameRemapMap[tr.getTargetFrame()]));
    }
        
    return models::RuntimePlugin::configure();
}

models::RuntimePlugin* models::TransformerPlugin::getNewInstance()
{
    return new TransformerPlugin();
}

const std::vector< models::Transformation > models::TransformerPlugin::getNeededTransformations() const
{
    return transformations;
}

std::vector< std::string > models::TransformerPlugin::getTransformerFrames() const
{
    return frames;
}

const std::vector< models::Transformation > models::TransformerPlugin::getUnmappedTransformations() const
{
    return unMappedTransformations;
}


models::RuntimeModel::RuntimeModel(const models::Task& initialState) : taskState(initialState)
{
    
}

bool models::RuntimeModel::cleanup()
{
    return true;
}

bool models::RuntimeModel::configure()
{
    bool ret = true;
    for(const auto &p : plugins)
        ret &= p.second->configure();
    return ret;
}

bool models::RuntimeModel::recover()
{
    return true;
}

bool models::RuntimeModel::start()
{
    return true;
}

bool models::RuntimeModel::stop()
{
    return true;
}
