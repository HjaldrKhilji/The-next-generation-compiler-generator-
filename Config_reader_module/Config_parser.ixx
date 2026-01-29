
module;
#include <string>       // For std::string, .replace(), .length()
#include <iostream>     // For std::istream
#include <sstream>      // For std::istringstream
#include <spanstream>   // For std::spanstream (used in your helper functions)
#include <vector>       // For initializer lists in escape_string
#include <memory>       // For std::unique_ptr
#include <stdexcept>    // For std::runtime_error
#include <utility>      // For std::move
// I used AI to keep track of the headers needed

export module Config_parser;

import All_declarations;

export   namespace config_parsing_tools {

    namespace Config_reader_helper {
   
        class Function_object_to_escape_escape_charactors { //lambda has to be used with common_functions::escape_string() function
        public:
            std::string string_to_be_replaced_with;
            size_t size_of_the_replacement_of_escape_string;

            size_t size_of_escape_charactor;
            Function_object_to_escape_escape_charactors(std::string y, size_t x) : string_to_be_replaced_with{
              y
            },
                size_of_the_replacement_of_escape_string{
                  y.length()
            },
                size_of_escape_charactor{
                  x
            } {
            }
            template<typename config>
         inline void operator()(std::spanstream* line_stream, absolute_base::All_non_terminal_entries<config>* all_entries,std::string* input_string, size_t* where_is_it_found) {
                input_string->replace(*where_is_it_found,
                    size_of_escape_charactor, std::move(string_to_be_replaced_with));
                *where_is_it_found += size_of_the_replacement_of_escape_string;
            }
        };



         template<typename config>
         inline void escape_backslash_capital_delimeter_by_reading_nested_symbols(std::spanstream* line_stream, absolute_base::All_non_terminal_entries<config>* all_entries,std::string* input_string, size_t* where_is_it_found) {
           
            constexpr size_t size_of_common_escape_charactors = 2;

            std::string temp_input = get_raw_input(line_stream);

            input_string->replace(
                *where_is_it_found,
                size_of_common_escape_charactors,
                std::move(temp_input));

        };


         template<typename config>
         inline void escape_backslash_capital_a_by_reading_nested_symbols(std::spanstream* line_stream, absolute_base::All_non_terminal_entries<config>* all_entries,std::string* input_string, size_t* where_is_it_found) {
           
            constexpr size_t size_of_common_escape_charactors = 2;

            uint64_t name = absolute_base::read_number_from_string_at_a_position<uint64_t>(line_stream);
            all_entries->add_nested_non_term_symbol_to_the_newest_entry(name);
            input_string->replace(
                *where_is_it_found,
                size_of_common_escape_charactors, "");


            semantical_analyzer_entry_reader();

        }
        template<typename config>
         inline void escape_backslash_capital_u_by_reading_nested_symbols(std::spanstream* line_stream, absolute_base::All_non_terminal_entries<config>* all_entries,std::string* input_string, size_t* where_is_it_found) {
           
            constexpr size_t size_of_common_escape_charactors = 2;

            uint64_t name = absolute_base::read_number_from_string_at_a_position<uint64_t>(line_stream);
            std::string& the_nested_non_term_entry_pattern = all_entries->get_pattern_of_nested_non_term_symbol_pattern(name);
            input_string->replace(
                *where_is_it_found,
                size_of_common_escape_charactors, the_nested_non_term_entry_pattern);

            where_is_it_found += the_nested_non_term_entry_pattern.length();

        }
        
          
        template<typename config>
         inline void parse_config_for_semantic_entry(std::spanstream* line_stream,absolute_base::Settings_for_semantical_rules* settings_for_current_config, unsigned int* minimum_amount_of_Matches, unsigned int* maximum_amount_of_matches) {
           
            using absolute_base::Settings_for_semantical_rules;
            char parse_config_one_by_one;

            *line_stream >> parse_config_one_by_one;
            switch (parse_config_one_by_one) {
            case '!':
                *settings_for_current_config |= Settings_for_semantical_rules::check_dont_exist;
                *settings_for_current_config ^= Settings_for_semantical_rules::check_exist;
                *line_stream >> parse_config_one_by_one;

            case '{':
                *line_stream >> *minimum_amount_of_Matches;
                *line_stream >> parse_config_one_by_one;

            case ',':
                *line_stream >> parse_config_one_by_one;
                *settings_for_current_config |= Settings_for_semantical_rules::check_exact;
                *settings_for_current_config ^= Settings_for_semantical_rules::check_atleast;
                *line_stream >> *maximum_amount_of_matches;

            case '}':
                break;
            default:
                throw std::runtime_error{ "CONFIG_PARSER: parse_config_for_semantic_entry error" };
                break;
            }

        }
        template<typename config>
        inline absolute_base::Semantical_analyzer_config_entry return_semantical_analyzer_entry(std::spanstream* line_stream, absolute_base::All_non_terminal_entries* all_entries) {
            

            
            uint64_t non_terminal_name_to_search_inside = absolute_base::read_number_from_string_at_a_position<uint64_t>(line_stream);
            config semantic_pattern_to_check{};
            *line_stream >> semantic_pattern_to_check;
            unsigned int minimum_amount_of_Matches = 0;
            unsigned int maximum_amount_of_matches = 0; //only used if settings_for_semantic_rules dosent have check_atleast on.
            
            using absolute_base::Settings_for_semantical_rules;
            Settings_for_semantical_rules fully_parsed_config = Settings_for_semantical_rules::check_exist | Settings_for_semantical_rules::check_atleast;
            //notice by default check_exist and check_atleast are turned on
            parse_config_for_semantic_entry<config>(line_stream,&fully_parsed_config, &minimum_amount_of_Matches, &maximum_amount_of_matches);
            
            return absolute_base::Semantical_analyzer_config_entry{
              all_entries.get_parmenant_name_of_nested_non_term_symbol_pattern(non_terminal_name_to_search_inside),
                semantic_pattern_to_check,
                fully_parsed_config,
                minimum_amount_of_Matches,
                maximum_amount_of_matches
            };
            //this function can be used in Printer.ixx to help the user insert a new semantic entry to a non_term entry
        }
        template<typename config>
         inline void semantical_analyzer_entry_reader(std::spanstream* line_stream, absolute_base::All_non_terminal_entries<config>* all_entries) {
             
             
            while((*line_stream >> c)) {
           
             all_entries->add_semantic_rule_for_newest_sub_entry(return_semantical_analyzer_entry(line_stream, all_entries));
           }

         }
         std::string get_raw_input(std::istream* input_stream) {
           
            std::string raw_input{};
            delimeter = *input_stream;
            std::getline(*input_stream, delimeter);

            return raw_input;
        }
        template<typename config>
         void push_latest_entry_as_sub_entry_of_an_entry(absolute_base::All_non_terminal_entries<config>* all_entries,  absolute_base::Non_terminal_name_entry<config>* entry_to_push_it_into) {
            
            all_entries->add_a_child_to_entry(entry_to_push_it_into, *(all_entries->physical_end()));

        }
        template<typename config>
         void parse_raw_input(absolute_base::All_non_terminal_entries<config>* all_entries, std::spanstream *line_stream) {

           
            constexpr size_t size_of_common_escape_charactors = 2;
            uint64_t non_terminal_name = absolute_base::read_number_from_string_at_a_position<uint64_t>(line_stream);
            config non_terminal_pattern;
            *line_stream >> non_terminal_pattern;
            all_entries->add_non_term_symbol_name(non_terminal_name);
            
            all_entries->add_non_term_pattern_for_newest_entry("(" + non_terminal_pattern + ")");
        }
        };
        template<typename config>
        class Config_reader  {
           
        private:
            


            void change_current_line() {
              
                line_stream.clear();

                line_stream.str(get_raw_input(&input_stream));
                Config_reader_helper::parse_raw_input(&line_stream, &all_entries);

            }
            std::istringstream& current_line() {
                //I have yet to find a use for this function, but I thought it is important for the future maintainer to just have this very trivial function
                return line_stream;
            }
        public:
            void get_and_parse_input() {

                do {
                    try {
                        change_current_line();

                    }
                    catch (...) {
                        throw std::string{ "CONFIG_PARSER: totally unexpected error" };
                    }
                } while (line_stream.good() == true);
                line_stream.clear();
                line_stream.str("");
            } // parse the whole file


      

            void print_all_parsed_input_for_testing()  {
                

                all_entries.print_all_content();
            }


            
            Config_reader(std::istream* a) :input_stream(a) { }
            
            Config_reader(Config_reader&) = default;
            Config_reader(Config_reader&&) = default;
             absolute_base::All_non_terminal_entries& get_all_entries_by_l_reference() const {
                return all_entries;
            }
        private:
            absolute_base::All_non_terminal_entries_implementation<config> all_entries{};
            std::istringstream line_stream{ "" };
            std::unique_ptr<std::istream> input_stream;
           char delimeter;
            
        };
    }  
 
