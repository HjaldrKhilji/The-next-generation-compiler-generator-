cmake_minimum_required(VERSION 3.15)
# Good practice: specify the language CXX (C++)
project(CompilerGeneratorTest CXX) 
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#  Explicitly listing the two required module files for "reliability".
set(MODULE_SOURCES 
    Base_modules/all_declarations.ixx
    Config_reader_module/Config_parser.ixx
    Input_modules/Inputer.ixx
    Output_modules/Printer.ixx
    Driver_module/Driver_module.ixx

)
# Source files from the 'src' directory
file(GLOB SRC_FILES "Base_modules/src/*.cpp")


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

