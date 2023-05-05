# orogen_model_exporter
This package provides:
  1. an oroGen plugin that exports the task model to a yaml file
  2. a command line tool `rock-instantiate` that allows to determine the yaml-model for components with dynamic ports

The default installation directory for the yaml models is `share/orogen/models`.

#### Example YAML format
```yaml
---
canbus:
  Task:
    inputPorts:
    - Name: in
      Type: "::canbus::Message"
    outputPorts:
    - Name: stats
      Type: "::canbus::Statistics"
    - Name: state
      Type: boost::int32_t
    dynamicInputPorts: []
    dynamicOutputPorts:
    - Name: "/\\w+/"
      Type: "::canbus::Message"
    properties:
    - Name: device
      Type: "::std::string"
    - Name: deviceType
      Type: "::canbus::DRIVER_TYPE"
    - Name: checkBusOkInterval
      Type: boost::int32_t
      DefaultVal: 100
    - Name: statsInterval
      Type: boost::int32_t
      DefaultVal: 1000
    operations:
    - Name: watch
      Doc: requires the canbus component to filter the incoming CAN messages for interesting
        messages and output them to a specified port
      Return:
        Type: bool
        Doc: returns true if the port could be created and false otherwise. The only
          failure reason is that the name is already in use
      Arguments:
      - Name: name
        Type: "::std::string"
        Doc: the name of the output port. The new port will be of type /canbus/Message.
      - Name: id
        Type: boost::int32_t
        Doc: only messages for which (can_msg.id & mask) == id will be written on
          the new port
      - Name: mask
        Type: boost::int32_t
        Doc: only messages for which (can_msg.id & mask) == id will be written on
          the new port
    - Name: unwatch
      Doc:
      Return:
        Type: bool
        Doc: true if the given name was a valid watch name, and false otherwise
      Arguments:
      - Name: name
        Type: "::std::string"
        Doc: the name of the watched device to remove
    - Name: getModelName
      Doc: returns the oroGen model name for this task
      Return:
        Type: "::std::string"
        Doc: ''
      Arguments: []
    - Name: __orogen_getTID
      Doc: returns the PID for this task
      Return:
        Type: boost::int32_t
        Doc: ''
      Arguments: []
    defaultActivity:
    - :fd_driven
    eventPorts:
    - in
```

#### Usage of `rock-instantiate`
```
Usage: rock-instantiate [options] <TASK_MODEL_NAME> <CONFIG_SECTIONS>

Instantiates a task model with a configuration and exports its oroGen component
model after applying the configuration.

    TASK_MODEL_NAME     Name of the model that should be instatiated. e.g. 
                        'joint_dispatcher::Task'
    CONFIG_SECTION      Comma-separated list of config sections to apply to the 
                        task model. It is assumed, that a matching config file
                        is available in the currently active bundle.
                        E.g.: 'section1,section2'

Options:
    -o, --output_file=FILEPATH       Specify output file to write model to. If not given, the resulting YAML string will be writen to STDOUT
```
