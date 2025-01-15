find_package( Boost COMPONENTS system regex filesystem)
rock_library(modelLib
    SOURCES 
        Argument.cpp
        Task.cpp
        Port.cpp
        Property.cpp
        TypedObject.cpp
        Operation.cpp
        DocumentedObject.cpp
        YAMLImporter.cpp
        RuntimeModel.cpp
    HEADERS 
        Task.hpp
        Port.hpp
        Property.hpp
        YAMLImporter.hpp
        RuntimeModel.hpp
        TypedObject.hpp
        DocumentedObject.hpp
        Operation.hpp
        Argument.hpp
    DEPS_PKGCONFIG
        lib_config
        yaml-cpp
    DEPS
        Boost::system Boost::filesystem
    )
    
rock_executable(ymlTest Main.cpp
    DEPS modelLib)
