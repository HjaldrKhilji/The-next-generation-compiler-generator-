cmake_minimum_required(VERSION 3.28) # Required for stable C++ module support

project(CompilerGeneratorTest CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the executable first
add_executable(compiler_test Test_code.cpp)

# Define Module Interfaces (the .ixx files)
# Using FILE_SET CXX_MODULES automatically handles build order/dependencies
target_sources(compiler_test
    PUBLIC
    FILE_SET CXX_MODULES FILES
        Base_modules/all_declarations.ixx
        Config_reader_module/Config_parser.ixx
        Input_modules/Inputer.ixx
        Output_modules/Printer.ixx
        Driver_module/Driver_module.ixx
)

# Define Implementation Source Files
# Fixed the 'file(SRC_FILES ...)' syntax error by using target_sources
target_sources(compiler_test
    PRIVATE
        "Base_modules/src/all_non_term_entries_implementation.cpp"
        "Base_modules/src/common_function_implementation.cpp"
        "Output_modules/src/helper_option_templates_for_managing_list_of_non_term_entries_implementation.cpp"
        "Output_modules/src/Non_nested_options.cpp"
)

# Set output directory
set_target_properties(compiler_test PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/complete_build"
)
