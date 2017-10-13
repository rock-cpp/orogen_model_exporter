require 'orogen_model_exporter'

class ModelExporterPlugin <  OroGen::Spec::TaskModelExtension

    @hasTasks
    
    def initialize()
        super("ModelExporterPlugin")
        @hasTasks = false
    end

    # implement extension for task
    def pre_generation_hook(task)
    end
    
    def post_generation_hook(task)
        moduleName = task.project.name
        taskName = task.basename
        modelYAML = export_orogen_to_yaml(task)
                
        template_dir = File.join(File.dirname(__FILE__), "../templates")

        cmake = Orocos::Generation.render_template template_dir, 'models', 'CMakeLists.txt', binding
        Orocos::Generation.save_automatic('models', "CMakeLists.txt", cmake)
        
        Orocos::Generation.save_automatic('models', "#{moduleName}::#{taskName}.yml", modelYAML)
        @hasTasks = true
    end
    
    def each_auto_gen_source_directory(&block)
        return enum_for(:each_test) unless block_given?
        if(@hasTasks)
            yield "models"
        end
    end

end

class OroGen::Spec::TaskContext
    def modelExport
        #puts("#{self.name}")
        if !find_extension("ModelExporterPlugin")
            register_extension(ModelExporterPlugin.new)
            #puts("Model Export active")
       end
    end
    
end
