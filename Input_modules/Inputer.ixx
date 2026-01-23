module;
#include <string>      // For std::string and string manipulation
#include <iostream>    // For std::istream
#include <vector>      // For std::vector (used in semantical_checks)
#include <stack>       // For std::stack (used for family_tree)
#include <memory>      // For std::shared_ptr
#include <stdexcept>   // For std::runtime_error
// I used AI to keep track of the headers needed

export module Inputer;
import All_declarations;
import estd_regex;

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
                raw_input.append(lineInput+"\n");

            }

            
            void check_regex_in_current_raw_input(const Non_terminal_name_entry& entry_to_match,
                const std::vector < Semantical_analyzer_config_entry >& semantical_checks

            ) {
                estd::regex pattern{ entry_to_match.pattern };
                estd::smatch match_info;
                for (; !estd::regex_search(raw_input, match_info, pattern); get_raw_input());
                if (semantical_checks.empty() != true) {
                    //if its empty then it means that we are in the node that has
                    //no sub nodes, hence no semantic rules.
                    //in this block, it is assumed to not be empty, hence we do the semantic checks as we usally will
                    try{
                        absolute_base::semantic_checks(semantical_checks, match_info[0].str());
                    }
                    catch (std::string failed_matches) {
                        throw std::string{ "SYNTAX PARSER ENGINE: syntax errors. raw syntax errors are:" + failed_matches };
                    }
                raw_input.erase(match_info.position(), match_info.length());
                
                    output_manager.print(match_info[0].str());
                
                

                //the work asked(by my ownself) for below is done:
                //OUTPUT SECTION:
                //feed match_info[0] to the output class, because only the output class gets to deal with what happens next
            }

            
        public:
            void scan_family_tree_regex_pattern_of_root(bool want_to_initialize_loop) {


                static std::stack<Siblings > family_tree{};
                if (want_to_initialize_loop == true) {
                    int current_sibling_index = (*all_config.begin()).sub_entries.size() - 1;
                    if (current_sibling_index == -1) {
                        //its a compiler error by definition
                        throw std::string{ "COMPILER: empty input config" };
                    }
                    Siblings current_generation =
                        { (*(all_config.begin())).sub_entries, current_sibling_index};
                    absolute_base::dig_to_the_leaves_of_the_family_tree(current_generation, &family_tree);
                    check_regex_in_current_raw_input(*(current_generation.get_current_sibling()), current_generation.get_semantic_rules_for_current_sibling());
                    scan_family_tree_regex_pattern_of_root(false);
                }
                else {
                    if (!family_tree.empty()) {
                        Siblings current_generation = family_tree.top();
                        family_tree.pop();
                        int current_sibling_index = current_generation.get_current_sibling_index();
                        absolute_base::dig_to_the_leaves_of_the_family_tree(current_generation, &family_tree);
                        check_regex_in_current_raw_input(*(current_generation.get_current_sibling()), current_generation.get_semantic_rules_for_current_sibling());
                        scan_family_tree_regex_pattern_of_root(false);

                    }
                    else {
                        if (raw_input.length() != 0) {
                            //its a compiler error by definition
                            throw std::string{ "COMPILER: empty input config" };
                        }
                    }
                }



            }



        
            using Input_stream_handler_ptr = absolute_base::Streamable_manager<std::istream, std::shared_ptr>;
            Input_reader(std::istream* a) = delete;
            Input_reader(Input_stream_handler_ptr a, const absolute_base::Base_printer& b, const absolute_base::All_non_terminal_entries& c) :input_stream{ a }, output_manager{ b }, all_config{ c } {}

            

            
            Input_reader(Input_reader&) = default;
            Input_reader(Input_reader&&) = default;
        private:
            absolute_base::Base_printer& output_manager;//this member should be passed the same istream as this class is
            absolute_base::All_non_terminal_entries&  all_config;
            std::string raw_input;
            const Input_stream_handler_ptr input_stream;
           
        };
    };
