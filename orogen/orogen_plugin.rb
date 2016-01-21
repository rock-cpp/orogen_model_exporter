require 'yaml'

class ModelExporterPlugin <  OroGen::Spec::TaskModelExtension

    # implement extension for task
    def pre_generation_hook(task)
    end
    
    def post_generation_hook(task)
        #Do the export
        puts("Post Gen")
        doc = Hash.new()
        
        moduleName = task.project.name
        taskName = task.basename
        doc[moduleName] = Hash.new()

        taskHash = Hash.new()
        inputPorts = Array.new()
        task.each_input_port do |port|
            portHash = Hash.new()
            portHash['Name'] = port.name
            portHash['Type'] = port.type.cxx_name
            inputPorts << portHash
        end
        taskHash['inputPorts'] = inputPorts

        outputPorts = Array.new()
        task.each_output_port do |port|
            portHash = Hash.new()
            portHash['Name'] = port.name
            portHash['Type'] = port.type.cxx_name
            outputPorts << portHash
        end
        taskHash['outputPorts'] = outputPorts

        properties = Array.new()
        task.each_property do |p|
            propHash = Hash.new()
            propHash['Name'] = p.name
            propHash['Type'] = p.type.cxx_name
            if(p.default_value)
                propHash['DefaultVal'] = p.default_value
            end
            properties << propHash
        end
        taskHash['properties'] = properties

        operations = Array.new()
        task.each_operation do |op|
            opHash = Hash.new()
            opHash['Name'] = op.name
            opHash['Doc'] = op.doc
            retHash = Hash.new()
            retHash['Type']= op.return_type[1]
            retHash['Doc'] = op.return_type[2]
            opHash['Return'] = retHash
            argList = Array.new()
            op.arguments.each do |arg|
                argHash = Hash.new()
                argHash['Name'] = arg[0]
                argHash['Type'] = arg[3]
                argHash['Doc'] = arg[2]
                
                argList << argHash
            end
            
            opHash['Arguments'] = argList
            
            operations << opHash
        end
        taskHash['operations'] = operations

        transformer = task.find_extension("transformer");
        if(transformer)
            taskHash['Plugins'] = {'transformer' => {'Frames' => transformer.available_frames.to_a,
                                   'Transformations' => transformer.needed_transformations.map{|t| {"From" => t.from, "To" => t.to} } }}
        end 
        

        doc[moduleName][taskName] = taskHash
        
        modelYML = Psych.dump(doc);
                
        # FIXME needs to be save in .orogen/models
        Orocos::Generation.save_generated(true, 'models', "#{moduleName}::#{taskName}_model.yml", modelYML)
        
        puts(modelYML)
        
    end
    
    def each_auto_gen_source_directory(&block)
        return enum_for(:each_test) unless block_given?
        yield "models"
    end

end

class OroGen::Spec::TaskContext
    def modelExport
        register_extension(ModelExporterPlugin.new)
        puts("Model Export active")
    end
    
end