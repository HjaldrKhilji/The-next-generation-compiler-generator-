cmake_minimum_required(VERSION 3.28) # Required for stable C++ module support
#i dont know much about cmake, hence I wrote a lot of comments to explain (to myself) the most basic things
project(CompilerGeneratorTest CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#script code for dependencies
#FETCHCONTENT_BASE_DIR is used by FetchContent to store the temporary directory in, if not provided then it just stores it where ever it wants (and that place might not last (for example the temp folder in windows))
#we could add a CACHE keyword, but that would mean that we cant get updates
#in either case the thing isnt fully downloaded if a new version isnt there
#but when using CACHE it is not downloaded even when a newer version exists.
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/dependencies")
#i could have used   GIT_TAG        v1.15.2 while setting FETCHCONTENT_BASE_DIR, but the issue with that
#is that boost.regex is very stable, so usng a git_tag and relying on a bot or another program to update that tag
#if possible is a waste of time


include(FetchContent)
FetchContent_Declare(
  regex_dependencies
  GIT_REPOSITORY GIT_REPOSITORY https://github.com/boostorg/regex.git
)
FetchContent_MakeAvailable(regex_dependencies)

#dependencies ended

#(notice the build name is compiler_test, and only the main project files are included in compiler_test ) 
# Define the executable first
add_executable(compiler_test Test_code.cpp)
target_link_libraries(compiler_test PRIVATE Boost::regex)

# Define Module Interfaces (the .ixx files)
# Using FILE_SET CXX_MODULES automatically handles build order/dependencies that C++ modules can have issues with
#any external library's cmake can use these files because of the PUBLIC keyword.
target_sources(compiler_test
    PUBLIC
    FILE_SET CXX_MODULES FILES
        Base_modules/all_declarations.ixx
        Config_reader_module/Config_parser.ixx
        Input_modules/Inputer.ixx
        Output_modules/Printer.ixx
        Driver_module/Driver_module.ixx
	regex_wrapper/regex_wrapper.ixx
	
)
# Define Module Interfaces (the .ixx files)
#any external library's cmake can not use these files because of the PRIVATE keyword.
target_sources(compiler_test
    PRIVATE
        "Base_modules/src/all_non_term_entries_implementation.cpp"
        "Base_modules/src/common_function_implementation.cpp"
        "Output_modules/src/helper_option_templates_for_managing_list_of_non_term_entries_implementation.cpp"
        "Output_modules/src/Non_nested_options.cpp"
	"Output_modules/src/helper_option_templates_for_arithmetic_related_options.cpp"
	"Output_modules/src/branch_and_loop.cpp"
)
message("HAPPY TO BE OF SERVICE!")

# Set output directory (where all the .exe files will go to)
set_target_properties(compiler_test PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
