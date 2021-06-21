INSTALL(FILES 
<%= 
   model_files = String.new
   task.project.self_tasks.each do |task| 
     model_files << "    #{task.name}.yml\n"
   end
   model_files
%>
    DESTINATION $ENV{OROGEN_MODEL_PATH})
