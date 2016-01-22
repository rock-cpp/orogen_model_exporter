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

bool models::TransformerPlugin::configure()
{
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
    return true;
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
