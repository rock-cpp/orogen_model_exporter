#include "YAMLImporter.hpp"
#include <iostream>



int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cout << "No file given" << std::endl;
        return 0;
    } 
    
    models::PluginStore::getInstace().registerPlugin(new models::TransformerPlugin());
    
    models::YAMLImporter importer;
//     models::Task t = importer.fromFile();
    models::RuntimeModel t = importer.getRunntimeModelfromFile(argv[1]);
    std::cout << t << std::endl;
    
    t.configure();
    
    return 0;
}