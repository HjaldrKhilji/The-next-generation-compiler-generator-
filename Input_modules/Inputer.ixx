module;
#include <string>      // For std::string and string manipulation
#include <iostream>    // For std::istream
#include <vector>      // For std::vector (used in semantical_checks)
#include <stack>       // For std::stack (used for family_tree)
#include <memory>      // For std::shared_ptr
#include <stdexcept>   // For std::runtime_error
#include <thread>
// For std::jthread

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
                std::getline(*input_stream, lineInput, *delimeter);
                raw_input.append(lineInput+"\n");

            }

            
            void do_operations(const Non_terminal_name_entry& entry_to_match,
                const std::vector < Semantical_analyzer_config_entry >& semantical_checks

            ) {
               
               
                for ( std::pair<bool, std::string> match{false, {}}; (match=entry_to_match.pattern.regex_search(raw_input), match.first()); get_raw_input());
                if (semantical_checks.empty() != true) {
                    //if its empty then it means that we are in the node that has
                    //no sub nodes, hence no semantic rules.
                    //in this block, it is assumed to not be empty, hence we do the semantic checks as we usally will
                    try{
                        absolute_base::semantic_checks(semantical_checks, match.second());
                    }
                    catch (std::string failed_matches) {
                        throw std::string{ "SYNTAX PARSER ENGINE: syntax errors. raw syntax errors are:" + failed_matches };
                    }
                raw_input.erase(match_info.position(), match_info.length());
                
                
                    output_manager.print(match_info[0].str()));
                }
                else {
                    output_manager.print(match_info[0].str());

                }

                //the work asked(by my ownself) for below is done:
                //OUTPUT SECTION:
                //feed match_info[0] to the output class, because only the output class gets to deal with what happens next
            }

            
        public:
            void parse_tree(bool want_to_initialize_loop) {


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
                    if(*multithreaded){
                            std::jthread t(()[] {
                             this->do_operations(*(current_generation.get_current_sibling()), current_generation.get_semantic_rules_for_current_sibling());
                             } 
                        }
                     else{do_operations(*(current_generation.get_current_sibling()), current_generation.get_semantic_rules_for_current_sibling());}
                     parse_tree(false);
                }
                else {
                    if (!family_tree.empty()) {
                        Siblings current_generation = family_tree.top();
                        family_tree.pop();
                        int current_sibling_index = current_generation.get_current_sibling_index();
                        absolute_base::dig_to_the_leaves_of_the_family_tree(current_generation, &family_tree);
                        if(*multithreaded){
                            std::jthread t(()[] {
                             this->do_operations(*(current_generation.get_current_sibling()), current_generation.get_semantic_rules_for_current_sibling());
                             } 
                        }
                     else{do_operations(*(current_generation.get_current_sibling()), current_generation.get_semantic_rules_for_current_sibling());}
                     parse_tree(false);

                    }
                    else {
                        if (raw_input.length() != 0) {
                            //its a compiler error by definition
                            throw std::string{ "COMPILER: empty input config" };
                        }
                    }
                }



            }



        
            using Input_stream_handler_ptr = absolute_base::Streamable_manager<std::istream, std::unique_ptr>;
            Input_reader(Input_stream_handler_ptr a, const absolute_base::Base_printer& b, const std::unique_ptr<absolute_base::All_non_terminal_entries<estd::regex_pattern>> c, std::unique_ptr<bool> d, std::unique_ptr<char> e) :input_stream{ a }, output_manager{ b }, all_config{ c }, multithreaded{ d }, delimeter{e} {}

            

            
            Input_reader(Input_reader&) = default;
            Input_reader(Input_reader&&) = default;
        private:
            const Input_stream_handler_ptr input_stream;
            const absolute_base::Base_printer& output_manager;//this member should be passed the same istream as this class is
            const std::unique_ptr<absolute_base::All_non_terminal_entries<estd::regex_pattern>>  all_config;
            const std::unique_ptr<bool> multithreaded;
            const std::unique_ptr<char> delimeter;
            std::string raw_input{};

        };
    };
