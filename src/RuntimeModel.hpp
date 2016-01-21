#pragma once

#include "Task.hpp"

namespace models
{

    
class RuntimePlugin
{
public:
    RuntimePlugin *getInstance();
    
    void setTaskModel(Task &model);
    
    
    
    virtual bool cleanup();
    virtual bool configure();
    virtual bool recover();
    virtual bool start();
    virtual bool stop();    
};

// #include <orocos_cpp/Configuration.hpp>

class ConfigurationPlugin : public RuntimePlugin
{
    //TODO Configuration class
    void applyConfig(std::vector<std::string> config);
};

class TransformerPlugin : public RuntimePlugin
{
    std::vector<std::pair<std::string, std::string>> transformations;
public:
    virtual bool configure();
    
    void getNeededTransformations();

    
    
    
};
/**
 * Modifies the task state accoring to the
 * predicted results of the executed actions on the model
 * */
class RuntimeModel
{
    Task taskState;
    
public:
    virtual bool cleanup();
    virtual bool configure();
    virtual bool recover();
    virtual bool start();
    virtual bool stop();
    
    const Task &getCurrentTaskState();
    
    void registerPlugin(RuntimePlugin *plugin);
    
};

}

