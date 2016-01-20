#include "YAMLImporter.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cout << "No file given" << std::endl;
        return 0;
    } 
    
    
    models::YAMLImporter importer;
    models::Task t = importer.fromFile(argv[1]);
    
    std::cout << t << std::endl;
    
    return 0;
}