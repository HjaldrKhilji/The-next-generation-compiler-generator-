cmake_minimum_required(VERSION 3.15)
# Good practice: specify the language CXX (C++)
project(CompilerGeneratorTest CXX) 
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#  Explicitly listing the two required module files for reliability.
# This avoids GLOB errors and ensures exactly the files you need are compiled.
set(MODULE_SOURCES 
    Modules/all_declarations.ixx
    Modules/base_class_for_all_parts_of_compiler_gen.ixx

)
# Source files from the 'src' directory
file(GLOB SRC_FILES "src/*.cpp")


# The main entry point file
set(MAIN_SOURCE Test_code.cpp)
# Combine all files into a single list
set(ALL_SOURCES
  ${MODULE_SOURCES}
    ${SRC_FILES}
    ${MAIN_SOURCE}
)

add_executable(compiler_test ${ALL_SOURCES})
#Set the RUNTIME_OUTPUT_DIRECTORY property. 
# This tells CMake to place the compiled executable (compiler_test) 
# into the 'complete_build' folder inside your CMake build directory.
set_target_properties(compiler_test PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "complete_build"
)

