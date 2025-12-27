module;
#include <iostream>

#include<functional>

#include <map>


#include <sstream>

#include <string>

#include <vector>
#include<regex>
#include<stack>
export module Inputer;
import All_declarations;
using absolute_base::Semantical_analyzer_config_entry;
using absolute_base::Non_terminal_name_entry;
using absolute_base::Siblings;
using absolute_base::Non_terminal_name_entry;
export namespace input_tools {
   
        class Input_reader {
        private:
            void get_raw_input() {
                std::string lineInput;
                std::getline(*input_stream, lineInput);
                raw_input.append(lineInput);

            }

            
            void check_regex_in_current_raw_input(const Non_terminal_name_entry& entry_to_match,
                const std::vector < Semantical_analyzer_config_entry >& semantical_checks

            ) {
                std::regex pattern{ entry_to_match.pattern };
                std::smatch match_info;
                for (; std::regex_search(raw_input, match_info, pattern); get_raw_input());
                if (semantical_checks.empty() != true) {
                    //if its empty then it means that we are in the node that has
                    //no sub nodes, hence no semantic rules.
                    if (absolute_base::semantic_checks(semantical_checks, match_info[0].str()) != true) {
                        throw std::runtime_error("semantic checks failed on stream of charactors of type: " + entry_to_match.name);
                    }
                }


                raw_input.erase(match_info.position(), match_info.length());
                output_manager.print(match_info[0].str(), entry_to_match);
                //the work asked(by my ownself) for below is done:
                //OUTPUT SECTION:
                //feed match_info[0] to the output class, because only the output class gets to deal with what happens next
            }

            
            
            void scan_family_tree_regex_pattern_of_root(bool want_to_initialize_loop) {


                static std::stack< Siblings > family_tree{};
                if (want_to_initialize_loop == true) {
                    int current_sibling_index = (*all_config.begin()).sub_entries.size() - 1;
                    if (current_sibling_index == -1) {
                        std::runtime_error("no root found");
                    }
                    Siblings current_generation =
                        { (*(all_config.begin())).sub_entries, current_sibling_index};
                    absolute_base::dig_to_the_leaves_of_the_family_tree(current_generation, &family_tree);
                    check_regex_in_current_raw_input(current_generation.get_current_sibling(), current_generation.get_semantic_rules_for_current_sibling());
                    scan_family_tree_regex_pattern_of_root(false);
                }
                else {
                    if (!family_tree.empty()) {
                        Siblings current_generation = family_tree.top();
                        family_tree.pop();
                        int current_sibling_index = current_generation.get_current_sibling_index();
                        absolute_base::dig_to_the_leaves_of_the_family_tree(current_generation, &family_tree);
                        check_regex_in_current_raw_input(current_generation.get_current_sibling(), current_generation.get_semantic_rules_for_current_sibling());
                        scan_family_tree_regex_pattern_of_root(false);

                    }
                    else {
                        if (raw_input.length() != 0) {
                            throw std::runtime_error{ "unidentified charactors at the end" };
                        }
                    }
                }



            }



        public:
            using Stream_handler_ptr = absolute_base::Streamable_manager<std::istream>;
            Input_reader(std::istream* a) = delete;
            Input_reader(Stream_handler_ptr a, const absolute_base::Base_printer& b, const absolute_base::All_non_terminal_entries& c) :input_stream{ a }, output_manager{ b }, all_config{ c } {}

            

            void parse_raw_input() {
                scan_family_tree_regex_pattern_of_root(true);

            }
            Input_reader(Input_reader&) = default;
            Input_reader(Input_reader&&) = default;
        private:
            absolute_base::Base_printer& output_manager;//this member should be passed the same istream as this class is
             absolute_base::All_non_terminal_entries&  all_config;
            std::string raw_input;
            const Stream_handler_ptr input_stream;
           
        };
    };
