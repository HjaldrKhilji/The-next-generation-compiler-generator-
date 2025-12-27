module;
#include <iostream>

#include <cstdlib>

#include <fstream>

#include <map>

#include <bitset>

#include <regex>

#include <sstream>

#include <string>

#include<stack>

#include <vector>

#include<functional>

#include<initializer_list>

export module Printer;
import All_declarations;
using absolute_base::Semantical_analyzer_config_entry;
using absolute_base::Non_terminal_name_entry;
using absolute_base::Siblings;


  
export namespace printing_tools {

    class Printer : public absolute_base::Base_printer {
    private:
        //I did not use function wrappers because of some wierd errors, that according to AI were because of my compile time environment, since I tried all fixes. In the future try using function wrappers istead, if possible.
        using Option_functions_wrapper_type = std::string::size_type (Printer::*)(const std::string&, std::string::size_type, std::string*);
        
        void additional_setup_for_family_tree() {
            int current_sibling_index = (*all_config.begin()).sub_entries.size() - 1;
            if (current_sibling_index == -1) {
                std::runtime_error("no root found");
            }
            Siblings current_generation =
            { (*all_config.begin()).sub_entries, current_sibling_index };
            absolute_base::dig_to_the_leaves_of_the_family_tree(current_generation, &family_tree);
        }

        //     ~functions to apply options on output data~


        std::string::size_type option_to_replicate_output(const std::string& output_config, std::string::size_type position, std::string* output_data_to_append_to) {
            int number_of_times_to_replicate;
            std::string::size_type
                charactors_processed = absolute_base::read_integer_from_string_at_a_position<int>(output_config, position, &number_of_times_to_replicate);
            for (int i = 0; i < number_of_times_to_replicate; i++) {
                *output_data_to_append_to += *output_data_to_append_to;

            }
            return position + charactors_processed;

        }

        std::string::size_type option_to_change_output_stream(const std::string& output_config, std::string::size_type position, std::string* output_data) {
            const char file_name_end_charactor = ':';
            size_t delimiter_position = output_config.find(file_name_end_charactor, position);
            std::string file_name = output_config.substr(position, delimiter_position);
            if (is_output_owned) {
                delete output;
            }
            is_output_owned = true;
            output = new std::ofstream{ file_name };
            return delimiter_position + 1;//+1 is to skip the delemiter_position index itself
        }

        std::string::size_type option_to_change_input_stream(const std::string& output_config, std::string::size_type position, std::string* output_data) {
            const char file_name_end_charactor = ':';
            size_t delimiter_position = output_config.find(file_name_end_charactor, position);
            std::string file_name = output_config.substr(position, delimiter_position);
            if (is_input_owned) {
                delete input;
            }
            input = new std::ifstream{ file_name };
            is_input_owned = true;
            return delimiter_position + 1;//+1 is to skip the delemiter_position index itself
        }

        std::string::size_type option_to_decrypt(const std::string& output_config, std::string::size_type position, std::string* output_data) {

            //      ~       ###TODO LATER###     ~
            return 0;

        }
        std::string::size_type option_to_encrypt(const std::string& output_config, std::string::size_type position, std::string* output_data) {

            //      ~       ###TODO LATER###     ~
            return 0;

        }
        std::string::size_type option_to_hash(const std::string& output_config, std::string::size_type position, std::string* output_data) {

            //      ~       ###TODO LATER###     ~
            return 0;

        }
        std::string::size_type option_to_remove_nested_entry_to_latest_sub_entry_for_output_config(const std::string& output_config, std::string::size_type position, std::string* output_data) {

        }    
        std::string::size_type option_to_remove_nested_entry_to_latest_sub_entry_for_input_config(const std::string& output_config, std::string::size_type position, std::string* output_data) {

        }
        std::string::size_type option_to_remove_nested_semantical_entry_to_latest_sub_entry_for_output_config(const std::string& output_config, std::string::size_type position, std::string* output_data) {

        }
        std::string::size_type option_to_remove_nested_semantical_entry_to_latest_sub_entry_for_input_config(const std::string& output_config, std::string::size_type position, std::string* output_data) {

        }
        std::string::size_type option_to_add_nested_entry_to_latest_sub_entry_for_output_config(const std::string& output_config, std::string::size_type position, std::string* output_data) {

        }
        std::string::size_type option_to_add_nested_entry_to_latest_sub_entry_for_input_config(const std::string& output_config, std::string::size_type position, std::string* output_data) {

        }
        std::string::size_type option_to_add_nested_semantical_entry_to_latest_sub_entry_for_output_config(const std::string& output_config, std::string::size_type position, std::string* output_data) {

        } 
        std::string::size_type option_to_add_nested_semantical_entry_to_latest_sub_entry_for_input_config(const std::string& output_config, std::string::size_type position, std::string* output_data) {

        }
        std::string::size_type print(const std::string& output_config, std::string::size_type position, std::string* output_data) {
        
        }
        //     ~end of functions to apply options on output data~


        void output_driver(std::string string_to_output, const Non_terminal_name_entry& output_config_entry, int current_generation) {
            if (absolute_base::semantic_checks(output_config_entry.all_semantical_analysis_rules[current_generation], string_to_output) != true) {
                throw std::runtime_error{ "semantic checks on output string failed!" };

            }
            std::string::size_type position = 0;

            std::string all_output_options_set = output_config_entry.output_config_data.substr(position, number_of_currently_defined_options);

            for (int i = 0; i < number_of_currently_defined_options; ++i) {
                if (all_output_options_set[i] == charactors_representing_each_option[i]) {
                    (this->*operations_upon_to_run_upon_charactors_found[i])(output_config_entry.output_config_data, position, &string_to_output);
                }

            }
            *output << string_to_output;


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
                ++number_of_currently_defined_options;
            }
            else {
                throw std::runtime_error{ "charactor representing your option is not available" };
            }
        }
        Printer(std::ostream* a,  absolute_base::All_non_terminal_entries& b, absolute_base::All_non_terminal_entries& c, std::istream* d, bool e, bool f) : output{ a }, all_config{b}, all_config_for_input{ c }, input{d}, is_output_owned{e}, is_input_owned{f}
        {
            additional_setup_for_family_tree();
        }
        ~Printer() {
            if (is_output_owned) {
                delete output;
            }
            if (is_input_owned) {
                delete input;
            }

        }
        Printer(Printer&) = default;
        Printer(Printer&&) = default;
    private:
        std::ostream* output;

        absolute_base::All_non_terminal_entries& all_config;
        absolute_base::All_non_terminal_entries& all_config_for_input;

        
        std::istream* input;
        bool is_output_owned;
        bool is_input_owned;
        std::stack< Siblings > family_tree{};

        //   ~VERY PRIVATE DATA MEMBERS, ONLY FOR IMPLEMENTORS OF THIS MODULE, USAGE:TO LOAD NEW FUNCTIONS FROM DYNAMICALLY LINKED LIBRARIES ~
        size_t number_of_currently_defined_options = 14;
        std::vector<Option_functions_wrapper_type> operations_upon_to_run_upon_charactors_found = {
              & Printer::option_to_replicate_output,
              & Printer::option_to_change_output_stream,
              & Printer::option_to_change_input_stream,
              & Printer::option_to_remove_nested_entry_to_latest_sub_entry_for_output_config,
              & Printer::option_to_remove_nested_entry_to_latest_sub_entry_for_input_config,
              & Printer::option_to_remove_nested_semantical_entry_to_latest_sub_entry_for_output_config,
              & Printer::option_to_remove_nested_semantical_entry_to_latest_sub_entry_for_input_config,
              & Printer::option_to_add_nested_entry_to_latest_sub_entry_for_output_config,
              & Printer::option_to_add_nested_entry_to_latest_sub_entry_for_input_config,
              & Printer::option_to_add_nested_semantical_entry_to_latest_sub_entry_for_output_config,
              & Printer::option_to_add_nested_semantical_entry_to_latest_sub_entry_for_input_config,
              & Printer::print,
              & Printer::option_to_decrypt,
              & Printer::option_to_encrypt,
              & Printer::option_to_hash
        };

        std::vector<char> charactors_representing_each_option = { '0','1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};




    };

};
