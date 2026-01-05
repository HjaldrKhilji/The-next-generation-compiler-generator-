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
#include<memory>
export module Printer;
import All_declarations;
import Config_parser;

using absolute_base::Semantical_analyzer_config_entry;
using absolute_base::Non_terminal_name_entry;
using absolute_base::Siblings;


  
export namespace printing_tools {

    namespace helper_templates_for_options {
        
       
        template<bool search>
        inline Non_terminal_name_entry* return_non_terminal_entry(const std::string& output_config, std::string::size_type* position, absolute_base::All_non_terminal_entries* list_of_entries_to_find_it_in);
        template<> 
         void return_non_terminal_entry<true>(const std::string& output_config, std::string::size_type* position, absolute_base::All_non_terminal_entries* list_of_entries_to_find_it_in) {
            std::string non_terminal_entry_name = absolute_base::read_string_from_string_at_a_position(output_config, position);
            auto entry_found = std::find_if(list_of_entries_to_find_it_in->begin(), list_of_entries_to_find_it_in->end(),
                [&non_terminal_entry_name](absolute_base::Non_terminal_name_entry current_entry) {
                    return current_entry.name == non_terminal_entry_name;
                }
            );
            if (entry_found != list_of_entries_to_find_it_in->end()) {
                throw std::runtime_error{ "semantic entry not found (PRINTER ERROR)" };

            }
            return entry_found;
        }
        template<>
        inline Non_terminal_name_entry* return_non_terminal_entry<false>(const std::string& output_config, std::string::size_type* position, absolute_base::All_non_terminal_entries* list_of_entries_to_find_it_in) {
            if (!list_of_entries_to_find_it_in->size()) {
                throw std::runtime_error{ "semantic entry not found (PRINTER ERROR)" };
            }
            return *(list_of_entries_to_find_it_in->begin());
        }



        template<bool check>
        inline int return_semantic_entry_helper(const std::string& output_config, std::string::size_type* position, const Sibling& entry_containing_semantic_entry, absolute_base::All_non_terminal_entries* list_of_entries_to_find_it_in);
        
        template<>
        int return_semantic_entry_helper<true>(const std::string& output_config, std::string::size_type* position, const Sibling& entry_containing_semantic_entry, absolute_base::All_non_terminal_entries* list_of_entries_to_find_it_in) {
            std::string semantic_entry_pattern = absolute_base::read_string_from_string_at_a_position(output_config, position);
            int index{ 0 };
            auto entry_found = std::find_if(entry_containing_semantic_entry->begin(), entry_containing_semantic_entry->end(),
                [&semantic_entry_pattern, &index](Non_terminal_name_entry current_entry) {
                    index++;
                    return current_entry.the_pattern_to_check == semantic_entry_pattern;
                }
            );
            if (entry_found != entry_containing_semantic_entry.end()) {
                throw std::runtime_error{ "semantic entry not found (PRINTER ERROR)" };
            }
            return index;
        }
        template<>
        inline int return_semantic_entry_helper<true>(const std::string& output_config, std::string::size_type* position, const Sibling& entry_containing_semantic_entry, Non_terminal_name_entry* list_of_entries_to_find_it_in) {
            if (!entry_containing_semantic_entry.size()) {
                throw std::runtime_error{ "semantic entry not found (PRINTER ERROR)" };
            }
            return entry_containing_semantic_entry.size()-1;
        }
        template<bool find_non_term_entry_using_number_or_name>
        int find_sub_entry_index(const Non_terminal_name_entry& entry, const std:string& output_config, int* position);
        template<>
        inline int find_sub_entry_index<true>(const Non_terminal_name_entry& entry, const std::string& output_config,int *position) {
           return common_functions::read_integer_from_string_at_a_position<int>(output_config, position);
            
        }
        template<>
            int find_sub_entry_index<false>(const Non_terminal_name_entry& entry, const std::string & output_config, , int* position) {
                std::string non_terminal_entry_name = absolute_base::read_string_from_string_at_a_position(output_config, position);
                int counter{ 0 };
                auto entry_found = std::find_if(entry.sub_entries.begin(), entry.sub_entries.end(),
                    [&non_terminal_entry_name, &counter](absolute_base::Non_terminal_name_entry current_entry) {
                        counter++;
                        return current_entry.get().name == non_terminal_entry_name;
                    }

                );
                if (entry_found != entry.sub_entries.end()) {
                    throw std::runtime_error{ "sub_entry not found (PRINTER ERROR)" };
                }
                return counter;
        }
            struct indexes_and_non_term_entry {
                int sibling_index;
                int semantic_entry_index;
                Non_terminal_name_entry* non_term_entry;
            };
        template<bool find_parent_entry,bool find_nested_non_term_entry_using_number_or_name, bool check_semantic_entry>
        indexes_and_non_term_entry return_semantic_entry_index(const std::string& output_config, std::string::size_type* position, absolute_base::All_non_terminal_entries* list_of_entries_to_find_it_in) {
        Non_terminal_name_entry* entry = return_non_terminal_entry<find_parent_entry>(output_config, position, list_of_entries_to_find_it_in);

        int sibling_index=
    read_integer_from_string_at_a_position<int>(output_config, 
        find_sub_entry_index<find_nested_non_term_entry_using_number_or_name>
        {*entry, *output_config, position}
        );
        Siblings entry_in_sibling_state{ entry->sub_entries, sibling_index };
        int semantic_index= return_semantic_entry_helper<check_semantic_entry>
     (output_config, position, entry_in_sibling_state, list_of_entries_to_find_it_in);
        return indexes{ sibling_index, semantic_index, entry };
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

        //     ~functions to apply options on output data~


        std::string::size_type option_to_replicate_output(const std::string& output_config, std::string::size_type position, std::string* output_data_to_append_to) {
            int number_of_times_to_replicate= absolute_base::read_integer_from_string_at_a_position<int>(output_config, &position);
            for (int i = 0; i < number_of_times_to_replicate; i++) {
                *output_data_to_append_to += *output_data_to_append_to;

            }
            return position + charactors_processed;

        }

        std::string::size_type option_to_change_output_stream(const std::string& output_config, std::string::size_type position, std::string* output_data) {
            
            
            std::string file_name = absolute_base::read_string_from_string_at_a_position(output_config, &position);
            
            output.switchToNewStream(new std::ofstream{ file_name });
            return delimiter_position + 1;//+1 is to skip the delemiter_position index itself
        }

        std::string::size_type option_to_change_input_stream(const std::string& output_config, std::string::size_type position, std::string* output_data) {
            std::string file_name = absolute_base::read_string_from_string_at_a_position(output_config, &position);
            input.switchToNewStream( new std::ifstream{ file_name });
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
        
        std::string::size_type print_output(const std::string& output_config, std::string::size_type position, std::string* output_data) {
            *output << output_data;
        }
        template<bool search, absolute_base::All_non_terminal_entries Printer::*list_of_entries_to_find_it_in>
        void remove_entry(const std::string& output_config, std::string::size_type* position, std::string* output_data) {
            
            Non_terminal_name_entry* non_term_entry = return_non_terminal_entry<search>(output_config, position, list_of_entries_to_find_it_in);
            this->list_of_entries_to_find_it_in->remove_entry(non_term_entry);

        }
        template<bool search, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>
        void add_entry(const std::string& output_config, std::string::size_type* position, std::string* output_data) {

            Non_terminal_name_entry* non_term_entry = return_non_terminal_entry<search>(output_config, position, list_of_entries_to_find_it_in);
            read_single_entry_and_push_it_as_the_sub_entry_an_the_entry_passed<this->list_of_entries_to_find_it_in>(non_term_entry);

        }

        template<bool find_parent_entry, bool find_nested_entry_technique, bool check_semantic_entry, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>
        void add_semantic_entry_to_non_term_entry_passed(const std::string& output_config, std::string::size_type* position, std::string* output_data) {
            indexes_and_non_term_entry info_needed = return_semantic_entry
                <find_parent_entry, find_nested_entry_technique, check_semantic_entry>
                (output_config, position, this->list_of_entries_to_find_it_in);
            std::span<char> sub_span{ output_config.data() + *position, output_config.size() - *position) };

            Semantical_analyzer_config_entry semantic_entry =
                config_parsing_tools::Config_reader_helper::return_semantical_analyzer_entry(
                    sub_span, this->list_of_entries_to_find_it_in
                );
        this->list_of_entries_to_find_it_in->add_semantic_rule_to_entry(info_needed.non_term_entry, std::move(semantic_entry), info_needed.sibling_index, info_needed.semantic_entry_index);
        
        }        
        template<bool find_parent_entry, bool find_nested_entry_technique, bool check_semantic_entry, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>

        void remove_semantic_entry_to_non_term_entry_passed(const std::string& output_config, std::string::size_type* position, std::string* output_data) {
            indexes_and_non_term_entry info_needed = return_semantic_entry
                <find_parent_entry, find_nested_entry_technique, check_semantic_entry>
                (output_config, position, this->list_of_entries_to_find_it_in);
            this->list_of_entries_to_find_it_in->remove_latest_semantic_rule_for_entry(info_needed.non_term_entry, info_needed.sibling_index, info_needed.semantic_entry_index);

        }

        //     ~end of functions to apply options on output data~


        void output_driver(std::string string_to_output, const Non_terminal_name_entry& output_config_entry, int current_generation) {
            if (absolute_base::semantic_checks(output_config_entry.all_semantical_analysis_rules[current_generation], string_to_output) != true) {
                throw std::runtime_error{ "semantic checks on output string failed!" };

            }
            std::string::size_type position = 0;

            std::string all_output_options_set = output_config_entry.output_config_data.substr(position, number_of_currently_defined_options);
            
            for (char option_charactor : all_output_options_set) {
                for (auto const& pair : operations_upon_to_run_upon_charactors_found) {
                    if (option_charactor == pair.first) {
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
              '1', & Printer::option_to_replicate_output,
              '2', & Printer::option_to_change_output_stream,
              '3', & Printer::option_to_change_input_stream,
              '4', & Printer::print_output,
              '5', & Printer::option_to_decrypt,
              '6', & Printer::option_to_encrypt,
              '7', & Printer::option_to_hash
        };




    };

};
