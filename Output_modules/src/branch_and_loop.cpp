module;
// Required for std::string and size_type
#include <string>

// Required for std::tuple and std::get
#include <tuple>

// Required for uint64_t fixed-width integers
#include <cstdint>

// Required if your accumulator uses engine-level regex
#include <regex>

// Recommended for the throw statements
#include <stdexcept>

// Required for the switch-case logic and any I/O
#include <iostream>
//used AI to track the header files
module Printer;


namespace printing_tools {
    namespace options{ 
namespace loop_and_branch_helpers {


     using printing_tools::helper_templates_for_options::helpers_for_arithmetic_options::Polymorphic_accumulator;


     
     bool do_polymorphic_comparisions(char comparision_operator, Polymorphic_accumulator x, Polymorphic_accumulator y) {
         switch (comparision_operator) {
         case 'A':
             return a == b;
         case 'B':
             return a != b;
         case 'C':
             return a <= b;
         case 'D':
             return a >= b;
         case 'E':
             return a < b;
         case 'F':
             return a > b;
         case 'G':
             return x.regex_match(b);//match x with the pattern b
         default:
             throw std::string{ "invalid comparision operator" };


         }
     }
 }
 
 template<bool source_is_output_config_or_output_data, bool store_in_hashed_or_non_hashed>
 void loop(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
     //the function is unfortunately rather big, I find that sad since I cant fix it without ruining performance or logic flow
     try {
         //cache for optimization:
         static bool loop_cache_empty = true;
         static std::tuple<uint64_t, char operator_name, uint64_t> loop_cache{ {},{},{} };
         using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;
         using printing_tools::helper_templates_for_options::helpers_for_arithmetic_options::Polymorphic_accumulator;
         std::string x{};
         char comparision_operator_name;
         std::string y{};
         if (loop_cache_empty) {
             x = read_from_string<uint64_t, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position);
             std::string& output_config_mutable = const_cast<std::string&>(output_config);

             comparision_operator_name = output_config_mutable.back();
             output_config_mutable.pop_back();
             --(*(static_cast<uint64_t*>(position)));
             y = read_from_string<uint64_t, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position);
             
         }
         else {
             x = loop_cache.at<0>();
             comparision_operator_name = loop_cache.at<1>();
             y = loop_cache.at<2>();

         }

         Polymorphic_accumulator x_value;
         Polymorphic_accumulator y_value;
         constexpr if (store_in_hashed_or_non_hashed) {
             x_value = all_variable_ordered_storage.at(x);
             y_value = all_variable_ordered_storage.at(y);

         }
         else {
             x_value = all_variable_ordered_storage.at(x);
             y_value = all_variable_hashed_storage.at(y);


         }

         bool true_or_false = loop_and_branch_helpers::do_polymorphic_comparisions(comparision_operator_name, x_value, y_value);
         
         if (true_or_false) {
             char body_option = output_config_mutable.back();
             char loop_option = output_config_mutable.back();
             output_config_mutable.push_back(body_option);
             output_config_mutable.push_back(loop_option);
             *(static_cast<uint64_t*>(position)) += 2;


             
         }
         else {
             output_config_mutable.pop_back();
             output_config_mutable.pop_back();
             *(static_cast<uint64_t*>(position))-=2;


             if (!loop_cache_empty) {
                 //emptying the  cache of this loop
                 loop_cache.at<0>().clear();
                 loop_cache.at<1>()='\0';
                 loop_cache.at<2>().clear();

             }


         }
         if (loop_cache_empty) {
             //filling up the cache
             loop_cache.at<0>() = x;
             loop_cache.at<1>() = comparision_operator_name;
             loop_cache.at<2>() = y;
         }
     }

     catch (std::string error_sent_by_reader)
     {
         throw std::string{ "LOOP: " + error_sent_by_reader };
     }
     catch (...) {
         throw std::string{ "LOOP:  totally unexpected error" };

     }

 }
 template<bool source_is_output_config_or_output_data, bool store_in_hashed_or_non_hashed>
 void branch(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
     try {
         using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;
         using printing_tools::helper_templates_for_options::helpers_for_arithmetic_options::Polymorphic_accumulator;
         std::string x = read_from_string<uint64_t, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position);
         std::string& output_config_mutable = const_cast<std::string&>(output_config);
         char comparision_operator_name = output_config_mutable.back();
         output_config_mutable.pop_back();
         std::string y = read_from_string<uint64_t, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position);
         Polymorphic_accumulator x_value;
         Polymorphic_accumulator y_value;
         constexpr if (store_in_hashed_or_non_hashed) {
              x_value = all_variable_ordered_storage.at(x);
              y_value = all_variable_ordered_storage.at(y);
            
         }
         else {
              x_value = all_variable_ordered_storage.at(x);
              y_value = all_variable_hashed_storage.at(y);
             

         }
         

         bool true_or_false= loop_and_branch_helpers::do_polymorphic_comparisions(comparision_operator_name, x_value, y_value);
         if (true_or_false) {
             char lastChar = output_config_mutable.back();
             output_config_mutable.pop_back();
             output_config_mutable.pop_back();
             output_config_mutable.push_back( lastChar); //letting the second last charactor be our branch.
             --(*(static_cast<uint64_t*>(position)));
         }
         else {
             output_config_mutable.pop_back();
             --(*(static_cast<uint64_t*>(position)));

         }
     }

     catch (std::string error_sent_by_reader)
     {
         throw std::string{ "BRANCH: " + error_sent_by_reader };
     }
     catch (...) {
         throw std::string{ "BRANCH:  totally unexpected error" };

     }

 }
}
}
