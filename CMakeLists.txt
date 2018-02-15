PROJECT(ModelExporter)
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x" )

rock_init(ModelExporter 0.1)
rock_standard_layout()

install(FILES ${PROJECT_SOURCE_DIR}/orogen/model/CMakeLists.txt
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/orogen/templates/models)

install(FILES ${PROJECT_SOURCE_DIR}/orogen/orogen_plugin.rb
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/orogen/plugins
        RENAME model_exporter_plugin.rb)

install(FILES ${PROJECT_SOURCE_DIR}/lib/orogen_model_exporter.rb
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/ruby/vendor_ruby/)

install(PROGRAMS bin/rock-instantiate 
        DESTINATION bin)
