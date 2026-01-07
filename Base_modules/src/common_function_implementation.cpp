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
      while ((position_of_the_match_found_last = input_string->find(
          strings_to_be_replaced[index],
          position_of_the_match_found_last)) != std::string::npos) {
        function_to_be_run_for_each[index](&input_string,
          &position_of_the_match_found_last);
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
     *line_stream.putback(c);
     for (; *line_stream >> std::noskipws >> c && (isalnum(c) || c == '_'); identifier += c);
     *line_stream.putback(c);
     if (identifier.empty()) {
         std::cerr << "empty non terminal symbol name" << std::endl;
         throw std::runtime_error{"read_identifier error"};
     }
     return identifier;

 }

 namespace helpers_for_read_number_from_string_at_a_position {
     template <typename T>
     concept floating_pointer = std::is_floating_point_v<T>;
     T return_t_if_number_smaller_than_long_double(T a, long double source) {

         if (std::isnan(source)) {
             //case 1: if source is an input by the user then the user typed nan 
             //case 2: if the source isnt an input then the source is just invalid due to some previous operation on source
             throw std::runtime_error("NAN floating point encountered");
         }
         if (source > std::numeric_limits<T>::max() || source < std::numeric_limits<T>::lowest()) {
             throw std::runtime_error{
                 "Error: Source floating point value (" + std::to_string(source) +
                 ") is outside the limits of the target type (Max: " +
                 std::to_string(std::numeric_limits<T>::max()) + ", Min: " +
                 std::to_string(std::numeric_limits<T>::min()) + ")"
             };
         }
         else { return  static_cast<T>(source); }
     }
     template <std::integral T>
     T return_t_if_number_smaller_than_long_long_int(T a, long long int source) {
         if (source > std::numeric_limits<T>::max() || source < std::numeric_limits<T>::min()) {
             throw std::runtime_error{
                 "Error: Source integer value (" + std::to_string(source) +
                 ") is outside the limits of the target type (Max: " +
                 std::to_string(std::numeric_limits<T>::max()) + ", Min: " +
                 std::to_string(std::numeric_limits<T>::min()) + ")"
             };
         }
         else { return static_cast<T>(source); }
     }
 }
 template <common_functions::Numeric T>
 T  common_functions::read_number_from_string_at_a_position(const std::string& source, std::string::size_type* position){
     using helpers_for_read_number_from_string_at_a_position;
     //      ~raw ugly optimization that i had to do for the sake of optimization~
    //try to ignore this ugly code as much as possible, and avoid touching it too, please, unless you have to ofcourse.
     const char* start_ptr = source.c_str() + position;
     char* end_ptr;
     constexpr int base_of_the_number_being_read = 10;
      if constexpr  (std::is_integral_v<T>) {
         auto number_read = std::strtoll(start_ptr, &end_ptr, base_of_the_number_being_read);
         static_cast<T>(*position) = end_ptr - start_ptr;
         return return_t_if_number_smaller_than_long_long_int<T>(number_read);
     }
      else if constexpr (std::is_floating_point_v<T>) {
         auto number_read = std::strtold(start_ptr, &end_ptr, base_of_the_number_being_read);
         static_cast<T>(*position) = end_ptr - start_ptr;
         return return_t_if_number_smaller_than_long_double<T>(number_read);

     }

    
     //      ~end of all raw ugly optimization that i had to do for the sake of optimization~ 
    
}
 std::string common_functions::read_string_from_string_at_a_position(const std::string& source_string, std::string::size_type* position) {
     char file_name_end_charactor = source_string[*position];
     size_t delimiter_position = source_string.find(file_name_end_charactor, *position);
     std::string file_name = source_string.substr(*position, delimiter_position);
     *position = delimiter_position;
     return file_name;
 }
