module;
// For string manipulation and positions
#include <string>
#include <string_view>

// For I/O operations and the spanstream used in parse_raw_input
#include <iostream>
#include <fstream>
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

    
    namespace helper_templates_for_options {
       template <typename Type_to_read>
       Type_to_read read_from_string(const std::string& output_config, std::string::size_type* position);
       template <>
       Type_to_read read_from_string<std::string>(const std::string& output_config, std::string::size_type* position) {
       
           return read_string_from_string_at_a_position(output_config, position);

       }
       template <>
       Type_to_read read_from_string<long long int>(const std::string& output_config, std::string::size_type* position) {

           return read_number_from_string_at_a_position(output_config, position);

       }
       template <>
       Type_to_read read_from_string<long double>(const std::string& output_config, std::string::size_type* position) {

           return read_number_from_string_at_a_position(output_config, position);

       }


        }

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
        using Option_functions_wrapper_type = std::string::size_type (Printer::*)(const std::string&, std::string::size_type, std::string*);
        
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
            if (absolute_base::semantic_checks(output_config_entry.all_semantical_analysis_rules[current_generation], string_to_output) != true) {
                throw std::runtime_error{ "semantic checks on output string failed!" };

            }
            std::string::size_type position = 0;

            
            for (char option_charactor :  output_config_entry.output_config_data) {
            ++position;//changing index in ouptut_config
                for (auto const& pair : operations_upon_to_run_upon_charactors_found) {
                    if (option_charactor == pair.first) {
                        ++position;//skipping the option charactor
                        (this->*pair.second)(output_config_entry.output_config_data, position, &string_to_output);
                        break; // We found the move; go to the next character in the config
                    }
                }
            }
            


        }

        bool print(std::string output_data, const Non_terminal_name_entry& output_config_entry) override {
            if (!family_tree.empty()) {
                Siblings current_generation = family_tree.top();
                family_tree.pop();
                int current_sibling_index = current_generation.get_current_sibling_index();

                absolute_base::dig_to_the_leaves_of_the_family_tree(current_generation, &family_tree);

                output_driver(std::move(output_data), output_config_entry, current_sibling_index);
            }
            else {
                throw std::runtime_error{ "nothing is left to output" };
            }

        }

    public:
        //function for the very special case of dynamically loading new options
        void add_a_new_option(Option_functions_wrapper_type operation_to_run, char charactor_representing_option) {
            std::vector<char>& vec = charactors_representing_each_option;
            if (std::find(vec.begin(), vec.end(), charactor_representing_option) == vec.end()) {
                vec.push_back(charactor_representing_option);
                operations_upon_to_run_upon_charactors_found.push_back(operation_to_run);
            }
            else {
                throw std::runtime_error{ "charactor representing your option is not available" };
            }
        }
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
            // ========================================================================
            // --- INPUT CONFIGURATION (&Printer::all_config_for_input) ---
            // ========================================================================

            // --- Entry Management (a-d) ---
            { 'a', &Printer::options::add_entry<true,  &Printer::all_config_for_input> },
            { 'b', &Printer::options::add_entry<false, &Printer::all_config_for_input> },
            { 'c', &Printer::options::remove_entry<true,  &Printer::all_config_for_input> },
            { 'd', &Printer::options::remove_entry<false, &Printer::all_config_for_input> },

            // --- Add Semantic Rules (1-8) ---
            { '1', &Printer::options::add_semantic_entry_to_non_term_entry_passed<true,  true,  true,  &Printer::all_config_for_input> },
            { '2', &Printer::options::add_semantic_entry_to_non_term_entry_passed<true,  true,  false, &Printer::all_config_for_input> },
            { '3', &Printer::options::add_semantic_entry_to_non_term_entry_passed<true,  false, true,  &Printer::all_config_for_input> },
            { '4', &Printer::options::add_semantic_entry_to_non_term_entry_passed<true,  false, false, &Printer::all_config_for_input> },
            { '5', &Printer::options::add_semantic_entry_to_non_term_entry_passed<false, true,  true,  &Printer::all_config_for_input> },
            { '6', &Printer::options::add_semantic_entry_to_non_term_entry_passed<false, true,  false, &Printer::all_config_for_input> },
            { '7', &Printer::options::add_semantic_entry_to_non_term_entry_passed<false, false, true,  &Printer::all_config_for_input> },
            { '8', &Printer::options::add_semantic_entry_to_non_term_entry_passed<false, false, false, &Printer::all_config_for_input> },

            // --- Remove Semantic Rules (A-H) ---
            { 'A', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<true,  true,  true,  &Printer::all_config_for_input> },
            { 'B', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<true,  true,  false, &Printer::all_config_for_input> },
            { 'C', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<true,  false, true,  &Printer::all_config_for_input> },
            { 'D', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<true,  false, false, &Printer::all_config_for_input> },
            { 'E', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<false, true,  true,  &Printer::all_config_for_input> },
            { 'F', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<false, true,  false, &Printer::all_config_for_input> },
            { 'G', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<false, false, true,  &Printer::all_config_for_input> },
            { 'H', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<false, false, false, &Printer::all_config_for_input> },

            // ========================================================================
            // --- OUTPUT CONFIGURATION (&Printer::all_config_for_output) ---
            // ========================================================================

            // --- Entry Management (e-h) ---
            { 'e', &Printer::options::add_entry<true,  &Printer::all_config_for_output> },
            { 'f', &Printer::options::add_entry<false, &Printer::all_config_for_output> },
            { 'g', &Printer::options::remove_entry<true,  &Printer::all_config_for_output> },
            { 'h', &Printer::options::remove_entry<false, &Printer::all_config_for_output> },

            // --- Add Semantic Rules (Symbols used to avoid collision with 1-8) ---
            { '!', &Printer::options::add_semantic_entry_to_non_term_entry_passed<true,  true,  true,  &Printer::all_config_for_output> },
            { '@', &Printer::options::add_semantic_entry_to_non_term_entry_passed<true,  true,  false, &Printer::all_config_for_output> },
            { '#', &Printer::options::add_semantic_entry_to_non_term_entry_passed<true,  false, true,  &Printer::all_config_for_output> },
            { '$', &Printer::options::add_semantic_entry_to_non_term_entry_passed<true,  false, false, &Printer::all_config_for_output> },
            { '%', &Printer::options::add_semantic_entry_to_non_term_entry_passed<false, true,  true,  &Printer::all_config_for_output> },
            { '^', &Printer::options::add_semantic_entry_to_non_term_entry_passed<false, true,  false, &Printer::all_config_for_output> },
            { '&', &Printer::options::add_semantic_entry_to_non_term_entry_passed<false, false, true,  &Printer::all_config_for_output> },
            { '*', &Printer::options::add_semantic_entry_to_non_term_entry_passed<false, false, false, &Printer::all_config_for_output> },

            // --- Remove Semantic Rules (I-P used to avoid collision with A-H) ---
            { 'I', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<true,  true,  true,  &Printer::all_config_for_output> },
            { 'J', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<true,  true,  false, &Printer::all_config_for_output> },
            { 'K', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<true,  false, true,  &Printer::all_config_for_output> },
            { 'L', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<true,  false, false, &Printer::all_config_for_output> },
            { 'M', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<false, true,  true,  &Printer::all_config_for_output> },
            { 'N', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<false, true,  false, &Printer::all_config_for_output> },
            { 'O', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<false, false, true,  &Printer::all_config_for_output> },
            { 'P', &Printer::options::remove_semantic_entry_to_non_term_entry_passed<false, false, false, &Printer::all_config_for_output> },

            // ========================================================================
            // --- GENERAL OPERATIONS ---
            // ========================================================================
            { 'R', &Printer::options::option_to_replicate_output },
            { 'S', &Printer::options::option_to_change_output_stream },
            { 'T', &Printer::options::option_to_change_input_stream },
            { 'U', &Printer::options::print_output }
        };

       




    };

};
