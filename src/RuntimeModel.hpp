#pragma once

#include "Task.hpp"

namespace models
{

    
class RuntimePlugin
{
    Task *model;
    std::string name;
public:
    RuntimePlugin(const std::string &name) : name(name) {};

    friend std::ostream& operator<< (std::ostream& s, const RuntimePlugin &p) 
    {
        p.printPluginData(s);
        return s;
    };
    
    virtual RuntimePlugin *getNewInstance() = 0;
    
    const std::string &getName() const
    {
        return name;
    };
    
    virtual ~RuntimePlugin() {};
    
    void setTaskModel(Task &modelp) {model = &modelp;};
    
    virtual bool cleanup() {return true;};
    virtual bool configure() {return true;};
    virtual bool recover() {return true;};
    virtual bool start() {return true;};
    virtual bool stop() {return true;};
protected :
    Task &getModel()
    {
        return *model;
    };
    
    virtual void printPluginData(std::ostream& s) const {};
};

class PluginStore
{
public:
    ~PluginStore();
    
    static PluginStore &getInstace();
    
    void registerPlugin(RuntimePlugin *plugin);

    RuntimePlugin *getNewPluginInstance(const std::string &name);
    
private:
    static PluginStore *instance;
    std::map<std::string, RuntimePlugin *> plugins;
    PluginStore();
};


// #include <orocos_cpp/Configuration.hpp>

class ConfigurationPlugin : public RuntimePlugin
{
    //TODO Configuration class
    void applyConfig(std::vector<std::string> config);
};

class TransformerPlugin : public RuntimePlugin
{
    friend class YAMLImporter;
    //Transformer information
    std::vector<std::string> frames;
    std::vector<Transformation> unMappedTransformations;
    std::vector<Transformation> transformations;
public:
    
    TransformerPlugin();
    
    virtual bool configure();
    
    virtual RuntimePlugin* getNewInstance();
    
    std::vector<std::string> getTransformerFrames() const;
    const std::vector<Transformation> getUnmappedTransformations() const;
    const std::vector<Transformation> getNeededTransformations() const;
    
    virtual void printPluginData(std::ostream& s) const 
    {
        s << "Known Frames : " << std::endl;
        for(const std::string &p: getTransformerFrames())
        {
            s << "    " << p << std::endl;
        }
        s << "Transformations : " << std::endl;
        for(const Transformation &p: getUnmappedTransformations())
        {
            s << "    " << p.getSourceFrame() << "2" << p.getTargetFrame() << std::endl;
        }
    }
};
/**
 * Modifies the task state accoring to the
 * predicted results of the executed actions on the model
 * */
class RuntimeModel
{
    friend class YAMLImporter;
    
    Task taskState;
    std::map<std::string, RuntimePlugin *> plugins;

public:
    RuntimeModel(const Task &initialState);
    
    virtual ~RuntimeModel() {};
    virtual bool cleanup();
    virtual bool configure();
    virtual bool recover();
    virtual bool start();
    virtual bool stop();
    
    const Task &getCurrentTaskState();
    
    void registerPlugin(RuntimePlugin *plugin);
    RuntimePlugin *getPlugin(const std::string &name);

    friend std::ostream& operator<<(std::ostream& s, const RuntimeModel &m)
    {
        s << m.taskState << std::endl;
        for(const auto it : m.plugins)
        {
            RuntimePlugin * pl = it.second;
            s << *pl << std::endl;
//             s << *(it.second) << std::endl;
        }
        
        return s;
    }
};

}

