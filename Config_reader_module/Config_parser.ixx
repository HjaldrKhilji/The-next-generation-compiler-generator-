
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
         inline void semantical_analyzer_entry_reader(std::spanstream* line_stream, absolute_base::All_non_terminal_entries<config>* all_entries, char delimeter, char charactor_to_escape_delimeter_with) {
             
             
            while((*line_stream >> c)) {
              
            uint64_t non_terminal_name_to_search_inside = absolute_base::read_number_from_string_at_a_position<uint64_t>(line_stream);
            config semantic_pattern_to_check{};
	        read_input(line_stream, semantic_pattern_to_check, delimeter, charactor_to_escape_delimeter_with, all_non_term_entries);
            unsigned int minimum_amount_of_Matches = 0;
            unsigned int maximum_amount_of_matches = 0; //only used if settings_for_semantic_rules dosent have check_atleast on.
            
            using absolute_base::Settings_for_semantical_rules;
            Settings_for_semantical_rules fully_parsed_config = Settings_for_semantical_rules::check_exist | Settings_for_semantical_rules::check_atleast;
            //notice by default check_exist and check_atleast are turned on
            parse_config_for_semantic_entry<config>(line_stream,&fully_parsed_config, &minimum_amount_of_Matches, &maximum_amount_of_matches);
            
             all_entries->add_semantic_rule_for_newest_sub_entry(absolute_base::Semantical_analyzer_config_entry{
              all_entries.get_parmenant_name_of_nested_non_term_symbol_pattern(non_terminal_name_to_search_inside),
                semantic_pattern_to_check,
                fully_parsed_config,
                minimum_amount_of_Matches,
                maximum_amount_of_matches
            });
           }

         }
         std::string get_raw_input(std::istream* input_stream, char *delimeter) {
           
            std::string raw_input{};
            std::getline( *input_stream,raw_input, *delimeter);

            return raw_input;
        }
        template<typename config>
         void push_latest_entry_as_sub_entry_of_an_entry(absolute_base::All_non_terminal_entries<config>* all_entries,  absolute_base::Non_terminal_name_entry<config>* entry_to_push_it_into) {
            
         
            all_entries->add_a_child_to_entry(entry_to_push_it_into, *(all_entries->physical_end()));

        }
        template<typename config>
         void parse_raw_input(absolute_base::All_non_terminal_entries<config>* all_entries, std::spanstream *line_stream, char delemeter,  char charactor_to_escape_delimeter_with) {

           
            constexpr size_t size_of_common_escape_charactors = 2;
            uint64_t non_terminal_name = absolute_base::read_number_from_string_at_a_position<uint64_t>(line_stream);
            config non_terminal_pattern;
	        read_input(line_stream, non_terminal_pattern, delimeter, charactor_to_escape_delimeter_with, all_non_term_entries);
            all_entries->add_non_term_symbol_name(non_terminal_name);
            
            all_entries->add_non_term_pattern_for_newest_entry(non_terminal_pattern);
        }
        };
        template<typename config>
        class Config_reader  {
           
        private:
            


            void change_current_line() {
              
                line_stream.clear();

                line_stream.str(get_raw_input(&input_stream, &delimeter));
                Config_reader_helper::parse_raw_input(&all_entries, &line_stream, delimeter, escape_charactor_for_delimeter);

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


            
            Config_reader(std::istream* a, char b, char c) :input_stream{a}, delimeter{b}, escape_charactor_for_delimeter{C}  { }
            
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
           char escape_charactor_for_delimeter;//\escape_charactor_for_delimeter will become delimeter
            
        };
    }  
 
