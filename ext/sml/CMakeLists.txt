project(sml LANGUAGES CXX)

add_library(sml INTERFACE)
add_library(inviwo::sml ALIAS sml)

target_include_directories(sml INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

#  HACK: have the files showing in the IDE
add_custom_target(sml_dummy SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include/sml/sml.hpp)
# Creates VS folder structure
ivw_folder(sml_dummy ext)