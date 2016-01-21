#pragma once

#include "Task.hpp"

namespace models
{

    
class RuntimePlugin
{
public:
    void setTaskModel(Task &model);
    
    virtual bool cleanup();
    virtual bool configure();
    virtual bool recover();
    virtual bool start();
    virtual bool stop();    
};

class ConfigurationPlugin : public RuntimePlugin
{
    void applyConfig();
};

class TransformerPlugin : public RuntimePlugin
{
    virtual bool configure();
    
    void getNeededTransforms();
    
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

