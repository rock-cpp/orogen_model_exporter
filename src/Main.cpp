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
    
    //Fake a remap
    const models::Property &p =  t.getCurrentTaskState().getProperty("body_center_frame");
    
    models::Property &p2 = const_cast<models::Property &>(p);
    
    std::shared_ptr<libConfig::SimpleConfigValue> conf(new libConfig::SimpleConfigValue("FooFrame"));
    conf->setName("body_center_frame");
    
    p2.setValue(conf);
    
    t.configure();

    std::cout << "After Configure " << std::endl;
    
    std::cout << t << std::endl;

    
    return 0;
}