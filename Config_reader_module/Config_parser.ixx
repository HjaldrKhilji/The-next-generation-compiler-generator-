
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

	struct line_stream{
	std::string string_buffer;
	std::string::size_type position;
	void operator>>(uint64_t& dest) const {
	dest= absolute_base::read_number_from_string_at_a_position<uint64>(string_buffer, &position);
	}
	void operator>>(std::string& dest) const {
	dest= absolute_base::read_number_from_string_at_a_position<std::string>(string_buffer, &position);
	}
	void operator>>(char &dest) const {
	dest= string_buffer[position];
	static_cast<uint64_t>(position)++;
	}
	};

    namespace Config_reader_helper {
  
          
        template<typename config>
         inline void parse_config_for_semantic_entry(const line_stream& input_stream,absolute_base::Settings_for_semantical_rules* settings_for_current_config, unsigned int* minimum_amount_of_Matches, unsigned int* maximum_amount_of_matches) {
           
            using absolute_base::Settings_for_semantical_rules;
            char parse_config_one_by_one;

            line_stream >> parse_config_one_by_one;
            switch (parse_config_one_by_one) {
            case '!':
                *settings_for_current_config |= Settings_for_semantical_rules::check_dont_exist;
                *settings_for_current_config ^= Settings_for_semantical_rules::check_exist;
                line_stream >> parse_config_one_by_one;

            case '{':
                line_stream >> *minimum_amount_of_Matches;
                line_stream >> parse_config_one_by_one;

            case ',':
                line_stream >> parse_config_one_by_one;
                *settings_for_current_config |= Settings_for_semantical_rules::check_exact;
                *settings_for_current_config ^= Settings_for_semantical_rules::check_atleast;
                line_stream >> *maximum_amount_of_matches;

            case '}':
                break;
            default:
                throw std::runtime_error{ "CONFIG_PARSER: parse_config_for_semantic_entry error" };
                break;
            }

        }
        
        template<typename config>
         inline void semantical_analyzer_entry_reader(const line_stream& input_stream, absolute_base::All_non_terminal_entries<config>* all_entries, std::istream* extra_input) {
             
             
            while((char c, line_stream >> c, c)) {
              
            uint64_t non_terminal_name_to_search_inside;
			line_stream>>non_terminal_name_to_search_inside;
			config semantic_pattern_to_check{};
	        read_input(line_stream, semantic_pattern_to_check, all_non_term_entries, extra_input);
            unsigned int minimum_amount_of_Matches = 0;
            unsigned int maximum_amount_of_matches = 0; //only used if settings_for_semantic_rules dosent have check_atleast on.
            
            using absolute_base::Settings_for_semantical_rules;
            Settings_for_semantical_rules fully_parsed_config = Settings_for_semantical_rules::check_exist | Settings_for_semantical_rules::check_atleast;
            //notice by default check_exist and check_atleast are turned on
            parse_config_for_semantic_entry<config>(line_stream,&fully_parsed_config, &minimum_amount_of_Matches, &maximum_amount_of_matches);
            
             all_entries->add_semantic_rule_for_newest_sub_entry(absolute_base::Semantical_analyzer_config_entry{
                semantic_pattern_to_check,
                fully_parsed_config,
                minimum_amount_of_Matches,
                maximum_amount_of_matches
            });
           }

         }
         
        template<typename config>
         void push_latest_entry_as_sub_entry_of_an_entry(absolute_base::All_non_terminal_entries<config>* all_entries,  absolute_base::Non_terminal_name_entry<config>* entry_to_push_it_into) {
            
         
            all_entries->add_a_child_to_entry(entry_to_push_it_into, *(all_entries->physical_end()));

        }
        template<typename config>
        inline void parse_raw_input(absolute_base::All_non_terminal_entries<config>* all_entries, const line_stream& input_stream, std::istream* extra_input) {

            constexpr size_t size_of_common_escape_charactors = 2;
			uint64_t non_terminal_name;
            input_stream>>non_terminal_name;
			all_entries->add_non_term_symbol_name(non_terminal_name);
            config non_terminal_pattern;
	        read_input(line_stream, non_terminal_pattern, all_non_term_entries, input_stream, extra_input);
            all_entries->add_non_term_pattern_for_newest_entry(non_terminal_pattern);
        }
        };
        template<typename config>
        class Config_reader  {
           
        private:
            


            
        public:
            inline void get_and_parse_input() {

                do {
                    try {
						std::getline(input_stream, current_input.string_buffer, delimeter);
						current_input>>delimeter;
						Config_reader_helper::parse_raw_input(&all_entries, &line_stream, input_stream->get());

                    }
                    catch (...) {
                        throw std::string{ "CONFIG_PARSER: totally unexpected error" };
                    }
                } while (line_stream.good() == true);
                
            } 

      

            void print_all_parsed_input_for_testing()  {
                

                all_entries.print_all_content();
            }


            
            Config_reader(std::istream* a, char b) :input_stream{a}, delimeter{b} { }
            
            Config_reader(Config_reader&) = default;
            Config_reader(Config_reader&&) = default;
             absolute_base::All_non_terminal_entries& get_all_entries_by_l_reference() const {
                return all_entries;
            }
        private:
            absolute_base::All_non_terminal_entries<config> all_entries{};
            line_stream current_input{{},{0}};
            std::unique_ptr<std::istream> input_stream;
           char delimeter;
            
        };
    }  
 
