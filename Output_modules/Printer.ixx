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
            if (absolute_base::semantic_checks(output_config_entry.all_semantical_analysis_rules[current_generation], string_to_output) != true) {
                throw std::runtime_error{ "semantic checks on output string failed!" };

            }
            std::string::size_type position = 0;
            std::string::size_type output_data_position = 0;
            
            for (char option_charactor :  output_config_entry.output_config_data) {
            ++position;//changing index in ouptut_config
            bool any_option_matched = false;
                for (auto const& pair : operations_upon_to_run_upon_charactors_found) {
                    if (option_charactor == pair.first) {
                        ++position;//skipping the option charactor found
                        (this->*pair.second)(output_config_entry.output_config_data, &position, &string_to_output, &output_data_position);
                        any_option_matched = true;
                        break; // We found the move; go to the next character in the config
                    
                }

                }
                if (!any_option_matched) {
                    throw std::runtime_error{ "this charactor is an invalid option: "+ option_charactor };
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
            // SECTION 1: SYSTEM & SEMANTIC MANAGEMENT 
            // ========================================================================
            { 'a', &options::add_entry<true,  &Printer::all_config_for_input> },
            { 'b', &options::add_entry<false, &Printer::all_config_for_input> },
            { 'c', &options::remove_entry<true,  &Printer::all_config_for_input> },
            { 'd', &options::remove_entry<false, &Printer::all_config_for_input> },
            { 'e', &options::add_entry<true,  &Printer::all_config_for_output> },
            { 'f', &options::add_entry<false, &Printer::all_config_for_output> },
            { 'g', &options::remove_entry<true,  &Printer::all_config_for_output> },
            { 'h', &options::remove_entry<false, &Printer::all_config_for_output> },
            { 'i', &options::subtract_from_output_data_position },
            { 'j', &options::trim_output_from_current_position_to_end }
            // Semantic Rules (Input: 1-8)
            { '1', &options::add_semantic_entry_to_non_term_entry_passed<true,  true,  true,  &Printer::all_config_for_input> },
            { '2', &options::add_semantic_entry_to_non_term_entry_passed<true,  true,  false, &Printer::all_config_for_input> },
            { '3', &options::add_semantic_entry_to_non_term_entry_passed<true,  false, true,  &Printer::all_config_for_input> },
            { '4', &options::add_semantic_entry_to_non_term_entry_passed<true,  false, false, &Printer::all_config_for_input> },
            { '5', &options::add_semantic_entry_to_non_term_entry_passed<false, true,  true,  &Printer::all_config_for_input> },
            { '6', &options::add_semantic_entry_to_non_term_entry_passed<false, true,  false, &Printer::all_config_for_input> },
            { '7', &options::add_semantic_entry_to_non_term_entry_passed<false, false, true,  &Printer::all_config_for_input> },
            { '8', &options::add_semantic_entry_to_non_term_entry_passed<false, false, false, &Printer::all_config_for_input> },

            // ========================================================================
            // SECTION 2: EXHAUSTIVE CALCULATOR (Every Source x Every Type)
            // ========================================================================

            // --- BLOCK 1: (Config, Config) [Source: true, true] ---
            { 'q', &options::calculator<long long,   long long,   true, true, '+'> },
            { 'w', &options::calculator<double,      double,      true, true, '+'> },
            { 'e', &options::calculator<std::string, std::string, true, true, '+'> },
            { 'r', &options::calculator<long long,   double,      true, true, '+'> },
            { 't', &options::calculator<double,      long long,   true, true, '+'> },
            { 'y', &options::calculator<std::string, long long,   true, true, '+'> },
            { 'u', &options::calculator<std::string, double,      true, true, '+'> },
            { 'i', &options::calculator<long long,   long long,   true, true, '-'> },
            { 'o', &options::calculator<double,      double,      true, true, '-'> },
            { 'p', &options::calculator<long long,   double,      true, true, '-'> },
            { 's', &options::calculator<double,      long long,   true, true, '-'> },
            { 'l', &options::calculator<long long,   long long,   true, true, '*'> },
            { 'm', &options::calculator<double,      double,      true, true, '*'> },
            { 'A', &options::calculator<long long,   long long,   true, true, '/'> },
            { 'B', &options::calculator<double,      double,      true, true, '/'> },

            // --- BLOCK 2: (Config, Output) [Source: true, false] ---
            { 'j', &options::calculator<long long,   long long,   true, false, '+'> },
            { 'k', &options::calculator<double,      double,      true, false, '+'> },
            { 'x', &options::calculator<std::string, std::string, true, false, '+'> },
            { 'v', &options::calculator<long long,   double,      true, false, '+'> },
            { 'n', &options::calculator<double,      long long,   true, false, '+'> },
            { 'z', &options::calculator<long long,   long long,   true, false, '-'> },
            { 'Q', &options::calculator<double,      double,      true, false, '-'> },
            { 'C', &options::calculator<long long,   double,      true, false, '-'> },
            { 'D', &options::calculator<double,      long long,   true, false, '-'> },
            { 'F', &options::calculator<long long,   long long,   true, false, '*'> },
            { 'G', &options::calculator<double,      double,      true, false, '*'> },
            { 'H', &options::calculator<long long,   long long,   true, false, '/'> },
            { 'I', &options::calculator<double,      double,      true, false, '/'> },

            // --- BLOCK 3: (Output, Config) [Source: false, true] ---
            { 'W', &options::calculator<long long,   long long,   false, true, '+'> },
            { 'E', &options::calculator<double,      double,      false, true, '+'> },
            { 'Y', &options::calculator<std::string, std::string, false, true, '+'> },
            { 'Z', &options::calculator<long long,   double,      false, true, '+'> },
            { 'M', &options::calculator<double,      long long,   false, true, '+'> },
            { 'V', &options::calculator<long long,   long long,   false, true, '-'> },
            { 'N', &options::calculator<double,      double,      false, true, '-'> },
            { 'J', &options::calculator<long long,   double,      false, true, '-'> },
            { 'K', &options::calculator<double,      long long,   false, true, '-'> },
            { 'L', &options::calculator<long long,   long long,   false, true, '*'> },
            { 'O', &options::calculator<double,      double,      false, true, '*'> },
            { 'P', &options::calculator<long long,   long long,   false, true, '/'> },
            { 'S', &options::calculator<double,      double,      false, true, '/'> },

            // --- BLOCK 4: (Output, Output) [Source: false, false] ---
            { '[', &options::calculator<long long,   long long,   false, false, '+'> },
            { ']', &options::calculator<double,      double,      false, false, '+'> },
            { '{', &options::calculator<std::string, std::string, false, false, '+'> },
            { '}', &options::calculator<long long,   double,      false, false, '+'> },
            { '|', &options::calculator<double,      long long,   false, false, '+'> },
            { ';', &options::calculator<std::string, long long,   false, false, '+'> },
            { ':', &options::calculator<std::string, double,      false, false, '+'> },
            { '"', &options::calculator<long long,   long long,   false, false, '-'> },
            { '<', &options::calculator<double,      double,      false, false, '-'> },
            { '>', &options::calculator<long long,   double,      false, false, '-'> },
            { '?', &options::calculator<double,      long long,   false, false, '-'> },
            { ',', &options::calculator<long long,   long long,   false, false, '*'> },
            { '.', &options::calculator<double,      double,      false, false, '*'> },
            { '~', &options::calculator<long long,   long long,   false, false, '/'> },
            { '`', &options::calculator<double,      double,      false, false, '/'> }
            //I used AI for the generation of this list, except for the ones below though:
            { '+', &options::polymorphic_calculator<'+'> }
            { '-', &options::polymorphic_calculator<'-'> }
            { '*', &options::polymorphic_calculator<'*'> }
            { '/', &options::polymorphic_calculator<'/'> }
            { '|', &options::polymorphic_calculator<'|'> }
            { '&', &options::polymorphic_calculator<'&'> }
            { '^', &options::polymorphic_calculator<'^'> }
            //note:any future options would now also have nested options
                     
};


    };

};
