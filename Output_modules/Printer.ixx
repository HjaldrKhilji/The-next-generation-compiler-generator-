module;
// For string manipulation and positions
#include <string>

#include<concepts>

#include<type_traits>


#include <spanstream> 

// For data structures
#include <vector>
#include <stack>
#include <map>
#include <utility> // For std::pair

// For memory management
#include <memory>

// For algorithms (std::find) and error handling
#include <algorithm>
#include <stdexcept>

// For span support (C++20)
#include <span>
// I used AI to keep track of the headers needed
export module Printer;
import All_declarations;
import Config_parser;

using absolute_base::Semantical_analyzer_config_entry;
using absolute_base::Non_terminal_name_entry;
using absolute_base::Siblings;


  
export namespace printing_tools {

    

    class Printer : public absolute_base::Base_printer {
    private:
        template<absolute_base::Non_terminal_name_entry* list_of_entries_to_push_it_in>
        void read_single_entry_and_push_it_as_the_sub_entry_an_the_entry_passed(absolute_Base::Non_terminal_name_entry* entry) {
            std::string input_line{};
            std::getline(*input, input_line);//note *input is passed by reference
            config_parsing_tools::Config_reader_helper::parse_raw_input(all_config_for_output.get(), std::spanstream{ std::move(input_line) });
            push_latest_entry_as_sub_entry_of_an_entry(this->list_of_entries_to_push_it_in, entry);
            //read a new entry function
            //used for adding new sub entries to the current entry processed using options
            //ONLY USE WHEN YOU WILL RUN push_latest_entry_as_sub_entry_of_an_entry AFTER THIS FUNCTION CALL
        }
        //I did not use function wrappers because of some wierd errors, that according to AI were because of my compile time environment, since I tried all fixes. In the future try using function wrappers istead, if possible.
        using Option_functions_wrapper_type = std::string::size_type (Printer::*)(const std::string&, std::string::size_type*, std::string*, std::string::size_type*);
        
        void additional_setup_for_family_tree() {
            int current_sibling_index = (*(all_config_for_output->begin())).sub_entries.size() - 1;
            if (current_sibling_index == -1) {
                std::runtime_error("no root found");
            }
            Siblings current_generation =
            { (*(all_config_for_output->begin())).sub_entries, current_sibling_index };
            absolute_base::dig_to_the_leaves_of_the_family_tree(current_generation, &family_tree);
        }

      

        void output_driver(std::string string_to_output, const Non_terminal_name_entry& output_config_entry, int current_generation) {
          
            std::string::size_type position = 0;
            std::string::size_type output_data_position = 0;
            
            for (char option_charactor :  output_config_entry.output_config_data) {
            ++position;//changing index in ouptut_config
            bool any_option_matched = false;
                for (auto const& pair : operations_upon_to_run_upon_charactors_found) {
                    if (option_charactor == pair.first) {
                        ++position;//skipping the option charactor found
                        //output_config_entry.output_config_data should never be changed to a local const variable (EVER!!!!)
                        //dont move output_config_entry.output_config_data as well into a local variable as well, you can copy but that would be expensive.
                        //these two warnings are the just in case ones.
                        (this->*pair.second)(output_config_entry.output_config_data, &position, &string_to_output, &output_data_position);
                        any_option_matched = true;
                        break; // We found the move; go to the next character in the config
                    
                }

                }
                if (!any_option_matched) {
                    //its a Compiler error by defintion
                    throw std::string{ "Compiler: invalid option character: "+ option_charactor };
                }
            }
            


        }

        bool print(std::string output_data) override {
            if (!family_tree.empty()) {
                Siblings current_generation = family_tree.top();
                family_tree.pop();
                int current_sibling_index = current_generation.get_current_sibling_index();

                absolute_base::dig_to_the_leaves_of_the_family_tree(current_generation, &family_tree);
                try {
                    absolute_base::semantic_checks(current_generation.get_semantic_rules_for_current_sibling(), output_data);
                }
                catch (std::string failed_matches) {
                    throw std::string{ "OPTION TO DO SEMANTIC CHECKS: SEMANTIC CHECKS ENGINE: semantic errors. raw semantic errors are:" + failed_matches };
                }
                output_driver(std::move(output_data), current_generation.get_current_sibling(), current_sibling_index);
            }
            else {
                //its a Compiler error by defintion
                throw std::string{ "Compiler: empty output config" };

            }

        }

    public:
        
        using Input_stream_handler_ptr = absolute_base::Streamable_manager<std::istream, std::shared_ptr>;
        using Output_stream_handler_ptr = absolute_base::Streamable_manager<std::ostream, std::unique_ptr>;
        Printer(Output_stream_handler_ptr a,  std::unique_ptr<absolute_base::All_non_terminal_entries> b, std::shared_ptr<absolute_base::All_non_terminal_entries> c, Input_stream_handler_ptr  d) : output{ a }, all_config_for_output{b}, all_config_for_input{ c }, input{d}
        {
            additional_setup_for_family_tree();
        }
        ~Printer() {
            

        }
        Printer(Printer&) = default;
        Printer(Printer&&) = default;
    private:
        

        Output_stream_handler_ptr output;

        std::unique_ptr<absolute_base::All_non_terminal_entries> all_config_for_output;
        std::shared_ptr<absolute_base::All_non_terminal_entries> all_config_for_input;

        
        Input_stream_handler_ptr input;
        
        std::stack< Siblings > family_tree{};

        std::vector<std::pair<char, Option_functions_wrapper_type>> operations_upon_to_run_upon_charactors_found = {
            // --- LOOP  (Source x Storage) ---
            { 'A', &options::loop<true,  true> },
            { 'B', &options::loop<true,  false> },
            { 'C', &options::loop<false, true> },
            { 'D', &options::loop<false, false> },

            // --- BRANCH  (Source x Storage) ---
            { 'E', &options::branch<true,  true> },
            { 'F', &options::branch<true,  false> },
            { 'G', &options::branch<false, true> },
            { 'H', &options::branch<false, false> },

            // --- STORE_VARIABLE  (Source x Storage) ---
            { 'I', &options::store_variable<true,  true> },
            { 'J', &options::store_variable<true,  false> },
            { 'K', &options::store_variable<false, true> },
            { 'L', &options::store_variable<false, false> },

            // --- GET_POLYMORPHIC  (Source x Storage) ---
            { 'M', &options::get_polymorphic<true,  true> },
            { 'N', &options::get_polymorphic<true,  false> },
            { 'O', &options::get_polymorphic<false, true> },
            { 'P', &options::get_polymorphic<false, false> },

            // --- ADD_DATA_TO_OUTPUT_CONFIG  (Source x Storage) ---
            { 'Q', &options::add_data_to_output_config<true,  true> },
            { 'R', &options::add_data_to_output_config<true,  false> },
            { 'S', &options::add_data_to_output_config<false, true> },
            { 'T', &options::add_data_to_output_config<false, false> },

            // --- ADD_SEMANTIC_ENTRY (8-Flag Permutations) ---
            { '1', &options::add_semantic_entry_to_non_term_entry_passed<true,  true,  true,  &Printer::all_config_for_input> },
            { '2', &options::add_semantic_entry_to_non_term_entry_passed<true,  true,  false, &Printer::all_config_for_input> },
            { '3', &options::add_semantic_entry_to_non_term_entry_passed<true,  false, true,  &Printer::all_config_for_input> },
            { '4', &options::add_semantic_entry_to_non_term_entry_passed<true,  false, false, &Printer::all_config_for_input> },
            { '5', &options::add_semantic_entry_to_non_term_entry_passed<false, true,  true,  &Printer::all_config_for_input> },
            { '6', &options::add_semantic_entry_to_non_term_entry_passed<false, true,  false, &Printer::all_config_for_input> },
            { '7', &options::add_semantic_entry_to_non_term_entry_passed<false, false, true,  &Printer::all_config_for_input> },
            { '8', &options::add_semantic_entry_to_non_term_entry_passed<false, false, false, &Printer::all_config_for_input> },

            // --- REMOVE_SEMANTIC_ENTRY (8-Flag Permutations) ---
            { '!', &options::remove_semantic_entry_to_non_term_entry_passed<true,  true,  true,  &Printer::all_config_for_input> },
            { '@', &options::remove_semantic_entry_to_non_term_entry_passed<true,  true,  false, &Printer::all_config_for_input> },
            { '#', &options::remove_semantic_entry_to_non_term_entry_passed<true,  false, true,  &Printer::all_config_for_input> },
            { '$', &options::remove_semantic_entry_to_non_term_entry_passed<true,  false, false, &Printer::all_config_for_input> },
            { '%', &options::remove_semantic_entry_to_non_term_entry_passed<false, true,  true,  &Printer::all_config_for_input> },
            { '^', &options::remove_semantic_entry_to_non_term_entry_passed<false, true,  false, &Printer::all_config_for_input> },
            { '&', &options::remove_semantic_entry_to_non_term_entry_passed<false, false, true,  &Printer::all_config_for_input> },
            { '*', &options::remove_semantic_entry_to_non_term_entry_passed<false, false, false, &Printer::all_config_for_input> },

            // --- CALCULATOR: ADDITION (ALL TYPE COMBINATIONS x 4 BOOLEAN PERMS) ---
            // long long + long double
            { 'a', &options::calculator<long long, long double, true,  true,  '+'> },
            { 'b', &options::calculator<long long, long double, true,  false, '+'> },
            { 'c', &options::calculator<long long, long double, false, true,  '+'> },
            { 'd', &options::calculator<long long, long double, false, false, '+'> },
            // long double + long long
            { 'e', &options::calculator<long double, long long, true,  true,  '+'> },
            { 'f', &options::calculator<long double, long long, true,  false, '+'> },
            { 'g', &options::calculator<long double, long long, false, true,  '+'> },
            { 'h', &options::calculator<long double, long long, false, false, '+'> },
            // std::string + long long
            { 'i', &options::calculator<std::string, long long, true,  true,  '+'> },
            { 'j', &options::calculator<std::string, long long, true,  false, '+'> },
            { 'k', &options::calculator<std::string, long long, false, true,  '+'> },
            { 'l', &options::calculator<std::string, long long, false, false, '+'> },
            // long long + std::string
            { 'm', &options::calculator<long long, std::string, true,  true,  '+'> },
            { 'n', &options::calculator<long long, std::string, true,  false, '+'> },
            { 'o', &options::calculator<long long, std::string, false, true,  '+'> },
            { 'p', &options::calculator<long long, std::string, false, false, '+'> },
            // std::string + long double
            { 'q', &options::calculator<std::string, long double, true,  true,  '+'> },
            { 'r', &options::calculator<std::string, long double, true,  false, '+'> },
            { 's', &options::calculator<std::string, long double, false, true,  '+'> },
            { 't', &options::calculator<std::string, long double, false, false, '+'> },
            // long double + std::string
            { 'u', &options::calculator<long double, std::string, true,  true,  '+'> },
            { 'v', &options::calculator<long double, std::string, true,  false, '+'> },
            { 'w', &options::calculator<long double, std::string, false, true,  '+'> },
            { 'x', &options::calculator<long double, std::string, false, false, '+'> },
            // long long + long long
            { 'y', &options::calculator<long long, long long, true,  true,  '+'> },
            { 'z', &options::calculator<long long, long long, true,  false, '+'> },
            { '0', &options::calculator<long long, long long, false, true,  '+'> },
            { '9', &options::calculator<long long, long long, false, false, '+'> },
            // long double + long double
            { '(', &options::calculator<long double, long double, true,  true,  '+'> },
            { ')', &options::calculator<long double, long double, true,  false, '+'> },
            { '[', &options::calculator<long double, long double, false, true,  '+'> },
            { ']', &options::calculator<long double, long double, false, false, '+'> },
            // std::string + std::string
            { '{', &options::calculator<std::string, std::string, true,  true,  '+'> },
            { '}', &options::calculator<std::string, std::string, true,  false, '+'> },
            { '<', &options::calculator<std::string, std::string, false, true,  '+'> },
            { '>', &options::calculator<std::string, std::string, false, false, '+'> },

            // --- CALCULATOR: MINUS (NUMERIC ONLY x 4 BOOLEAN PERMS) ---
            { 'v', &options::calculator<long long, long double, true,  true,  '-'> },
            { 'w', &options::calculator<long long, long double, false, false, '-'> },
            { 'x', &options::calculator<long double, long long, true,  true,  '-'> },
            { 'y', &options::calculator<long double, long long, false, false, '-'> },
            { 'z', &options::calculator<long long, long long, true,  true,  '-'> },
            { 'A', &options::calculator<long long, long long, false, false, '-'> },
            { 'B', &options::calculator<long double, long double, true,  true,  '-'> },
            { 'C', &options::calculator<long double, long double, false, false, '-'> },

            // --- CALCULATOR: MULTIPLY (NUMERIC ONLY x 4 BOOLEAN PERMS) ---
            { 'D', &options::calculator<long long, long double, true,  true,  '*'> },
            { 'E', &options::calculator<long long, long double, false, false, '*'> },
            { 'F', &options::calculator<long double, long long, true,  true,  '*'> },
            { 'G', &options::calculator<long double, long long, false, false, '*'> },
            { 'H', &options::calculator<long long, long long, true,  true,  '*'> },
            { 'I', &options::calculator<long long, long long, false, false, '*'> },
            { 'J', &options::calculator<long double, long double, true,  true,  '*'> },
            { 'K', &options::calculator<long double, long double, false, false, '*'> },

            // --- CALCULATOR: DIVIDE (NUMERIC ONLY x 4 BOOLEAN PERMS) ---
            { 'L', &options::calculator<long long, long double, true,  true,  '/'> },
            { 'M', &options::calculator<long long, long double, false, false, '/'> },
            { 'N', &options::calculator<long double, long long, true,  true,  '/'> },
            { 'O', &options::calculator<long double, long long, false, false, '/'> },
            { 'P', &options::calculator<long long, long long, true,  true,  '/'> },
            { 'Q', &options::calculator<long long, long long, false, false, '/'> },
            { 'R', &options::calculator<long double, long double, true,  true,  '/'> },
            { 'S', &options::calculator<long double, long double, false, false, '/'> },

            // --- SUBTRACT_POSITION / NESTED ---
            { 'U', &options::subtract_from_output_data_position<true> },
            { 'V', &options::subtract_from_output_data_position<false> },
            { 'W', &options::start_nested<true> },
            { 'X', &options::start_nested<false> },

            // --- ENTRY MODIFICATION ---
            { 'Y', &options::add_entry<true,  &Printer::all_config_for_input> },
            { 'Z', &options::add_entry<false, &Printer::all_config_for_input> },
            { '_', &options::remove_entry<true,  &Printer::all_config_for_input> },
            { '`', &options::remove_entry<false, &Printer::all_config_for_input> },

            // --- POLYMORPHIC OPS ---
            { '+', &options::polymorphic_calculator<'+'> },
            { '-', &options::polymorphic_calculator<'-'> },
            { '*', &options::polymorphic_calculator<'*'> },
            { '/', &options::polymorphic_calculator<'/'> },
            { '|', &options::polymorphic_calculator<'|'> },
            { '&', &options::polymorphic_calculator<'&'> },
            { '^', &options::polymorphic_calculator<'^'> },

            // --- CORE SYSTEM FUNCTIONS ---
            { 'p', &options::print_output },
            { 'r', &options::option_to_replicate_output },
            { 'o', &options::option_to_change_output_stream },
            { 'k', &options::option_to_change_input_stream },
            { 'h', &options::option_to_hash },
            { 't', &options::trim_output_from_current_position_to_end }
        };


    };

};
