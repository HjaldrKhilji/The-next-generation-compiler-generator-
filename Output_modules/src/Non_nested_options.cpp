module;

#include <string>   // For std::string and std::string::size_type
#include <fstream>  // For std::ofstream and std::ifstream
#include <utility>  // For std::move
#include <span>     // For std::span (requires C++20)

// I used AI to keep track of the headers needed 

module Printer;

//this file contains options that dont read user input to find other options at runtime

namespace options {
            //     ~functions to apply options on output data~


            std::string::size_type option_to_replicate_output(const std::string& output_config, std::string::size_type position, std::string* output_data_to_append_to) {
                int number_of_times_to_replicate = absolute_base::read_number_from_string_at_a_position<int>(output_config, &position);
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
                input.switchToNewStream(new std::ifstream{ file_name });
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
            template<bool search, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>
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

        }

