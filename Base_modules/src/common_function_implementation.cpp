module;

#include <string>       // For std::string, .find(), .substr(), std::to_string
#include <vector>       // For std::vector in escape_string
#include <spanstream>      // For std::spanstream
#include <iostream>     // For std::cerr, std::endl
#include <concepts>     // For std::integral and C++20 concept support
#include <type_traits>  // For std::is_floating_point_v, std::is_integral_v
#include <limits>       // For std::numeric_limits<T>::max(), ::min(), ::lowest()
#include <cmath>        // For std::isnan
#include <cctype>       // For isalnum (character classification)
#include <cstdlib>      // For std::strtoll and std::strtold
#include <stdexcept>    // For std::runtime_error
#include<charconv>
// I used AI to keep track of the headers needed 

module all_declarations;


 void common_functions::escape_string(std::string * input_string,
    const std::vector < std::string > & strings_to_be_replaced,
      const std::vector < common_functions::escape_charactor_function_wrapper_type >
        function_to_be_run_for_each) {
     for (size_t index = 0; index < strings_to_be_replaced.size(); index++) {
         //precondition: input_string, strings_to_be_replaced, function_to_be_run_for_each are in a valid state
         //precondition: the container "function_to_be_run_for_each" has the same size as strings_to_be_replaced
         //postconditions: all the matches of the strings inside  "strings_to_be_replaced"  are found in "input_string", 
         // and the position of that match along side a reference to the input_string is  passed to the
         // corrsponding function in function_to_be_run_for_each, corrsponding in this sentence means the function element from  function_to_be_run_for_each
         //that has the same index as the element whose match is found.
         //UGLY LOW LEVEL CODE ALERT: DONT TOUCH THIS UGLY CODE, UNLESS YOU REALLY REALLY HAVE TO CHANGE IT!!!!!!!
         size_t position_of_the_match_found_last = 0;
         try {
             while ((position_of_the_match_found_last = input_string->find(
                 strings_to_be_replaced[index],
                 position_of_the_match_found_last)) != std::string::npos) {
                 function_to_be_run_for_each[index](input_string,
                     &position_of_the_match_found_last);
             }
         }
         catch (std::bad_alloc) {
             //this is a very low level compiler error, in particular, it is an error in the usage of this tool or in the hardware/execution_environemnt of the user
             throw std::string{ "memory allocation failure" };
         }
     }
  }
 std::string common_functions::read_identifier(std::spanstream* line_stream) {
     //precondition: line_stream is in a valid state and is not empty
     //post condition: std::string preceded by list of ignored white spaces, and terminated by a non alpha numeric character that is not a underscore
     //is returned
     ///post condition : line_stream argument's state is changed to have the property "std::skipws" set.
     std::string identifier;
     char c;
     *line_stream >> std::skipws >> c; //skipping whitespaces
     line_stream->putback(c);
     for (; *line_stream >> std::noskipws >> c && (isalnum(c) || c == '_'); identifier += c);
     line_stream->putback(c);
     if (identifier.empty()) {
         throw std::runtime_error{"read_identifier error"};
     }
     return identifier;

 }

 template <common_functions::Numeric T>
T common_functions::read_number_from_string_at_a_position(const std::string& source, std::string::size_type* position) {
    T result;
    const char* start = source.data() + *position;
    const char* end = source.data() + source.size();

    auto [read_till, status] = std::from_chars(start, end, result);
    if (ec == std::errc{}) {
        //SUCCESSS
        // Advance the position based on how many characters were consumed
        *(static_cast<uint64_t*>(position)) += (read_till - start);

        return result;
    }
    else if (status == std::errc::invalid_argument) {
        throw std::runtime_error{ "Input read isnt a number" };
    } else if (status == std::errc::result_out_of_range) {
        throw std::runtime_error{ "Number exceeds type limits." };
    }
    else {
        throw std::runtime_error{ "unknown error occured" };
    }

    
}
 std::string common_functions::read_string_from_string_at_a_position(const std::string& source_string, std::string::size_type* position) {
char delimeter_charactor = source_string[*position];
  constexpr int number_to_add_to_index_to_skip_the_index_of_delimeter=1;
size_t delimiter_position = source_string.find(delimeter_charactor, *position+number_to_add_to_index_to_skip_the_index_of_delimeter);
try {
    std::string file_name = source_string.substr(*position, delimiter_position - *position);
}
catch(std::out_of_range){
    throw std::runtime_error{ "out of range read" };
}
    *position = delimiter_position+1;
     return file_name;
 }

 inline bool common_functions::convert_to_bool(std::string str) {
     return str == "1";
 }
 inline bool common_functions::convert_to_bool(const std::string& input_str, std::string::size_type* position) {
     bool result = input_str[*position] == '1';
     static_cast<uint64>_t& > (*position) += 1;
	 return result;

 }
