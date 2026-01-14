module;

#include <string>   // For std::string and std::string::size_type
#include <fstream>  // For std::ofstream and std::ifstream
#include <utility>  // For std::move
#include <span>     // For std::span (requires C++20)

// I used AI to keep track of the headers needed 

module Printer;

//this file contains options that dont read user input to find other options at runtime

namespace printing_tools {
    namespace options{
        //     ~functions to apply options on output data~


        std::string::size_type option_to_replicate_output(const std::string & output_config, std::string::size_type *position, std::string * output_data_to_append_to, std::string::size_type *output_data_position) {
            int number_of_times_to_replicate = absolute_base::read_number_from_string_at_a_position<int>(output_config, &position);
            for (int i = 0; i < number_of_times_to_replicate; i++) {
                *output_data_to_append_to += *output_data_to_append_to;

            }
            return position + charactors_processed;

        }

        std::string::size_type option_to_change_output_stream(const std::string & output_config, std::string::size_type *position, std::string * output_data, std::string::size_type* output_data_position) {


            std::string file_name = absolute_base::read_string_from_string_at_a_position(output_config, &position);

            output.switchToNewStream(new std::ofstream{ file_name });
            return delimiter_position + 1;//+1 is to skip the delemiter_position index itself
        }

        std::string::size_type option_to_change_input_stream(const std::string & output_config, std::string::size_type *position, std::string * output_data, std::string::size_type* output_data_position) {
            std::string file_name = absolute_base::read_string_from_string_at_a_position(output_config, &position);
            input.switchToNewStream(new std::ifstream{ file_name });
            return delimiter_position + 1;//+1 is to skip the delemiter_position index itself
        }

        std::string::size_type option_to_decrypt(const std::string & output_config, std::string::size_type *position, std::string * output_data, std::string::size_type* output_data_position) {

            //      ~       ###TODO LATER###     ~
            return 0;

        }
        std::string::size_type option_to_encrypt(const std::string & output_config, std::string::size_type *position, std::string * output_data, std::string::size_type* output_data_position) {

            //      ~       ###TODO LATER###     ~
            return 0;

        }
        std::string::size_type option_to_hash(const std::string & output_config, std::string::size_type *position, std::string * output_data, std::string::size_type* output_data_position) {

            //      ~       ###TODO LATER###     ~
            return 0;

        }

        std::string::size_type print_output(const std::string & output_config, std::string::size_type *position, std::string * output_data, std::string::size_type* output_data_position) {
            *output << output_data;
        }
        template<bool search, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>
        void remove_entry(const std::string & output_config, std::string::size_type * position, std::string * output_data, std::string::size_type* output_data_position) {

            Non_terminal_name_entry* non_term_entry = return_non_terminal_entry<search>(output_config, position, list_of_entries_to_find_it_in);
            this->list_of_entries_to_find_it_in->remove_entry(non_term_entry);

        }
        template<bool search, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>
        void add_entry(const std::string & output_config, std::string::size_type * position, std::string * output_data, std::string::size_type* output_data_position) {

            Non_terminal_name_entry* non_term_entry = return_non_terminal_entry<search>(output_config, position, list_of_entries_to_find_it_in);
            read_single_entry_and_push_it_as_the_sub_entry_an_the_entry_passed<this->list_of_entries_to_find_it_in>(non_term_entry);

        }

        template<bool find_parent_entry, bool find_nested_entry_technique, bool check_semantic_entry, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>
        void add_semantic_entry_to_non_term_entry_passed(const std::string & output_config, std::string::size_type * position, std::string * output_data, std::string::size_type* output_data_position) {
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

        void remove_semantic_entry_to_non_term_entry_passed(const std::string& output_config, std::string::size_type* position, std::string* output_data, , std::string::size_type* output_data_position) {
            indexes_and_non_term_entry info_needed = return_semantic_entry
                <find_parent_entry, find_nested_entry_technique, check_semantic_entry>
                (output_config, position, this->list_of_entries_to_find_it_in);
            this->list_of_entries_to_find_it_in->remove_latest_semantic_rule_for_entry(info_needed.non_term_entry, info_needed.sibling_index, info_needed.semantic_entry_index);

        }
        void trim_output_from_current_position_to_end(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
            output_data->erase(*output_data_position);
        }

        template<bool read_from_config_or_output>
        void subtract_from_output_data_position(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
        using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;
        //a function from helper_templates_for_options::helpers_for_arithmetic_options is ideally not to be used here, but I had to since I didnt know a better place to put this function in
        //in my defense it is okay, since I am doing some arithmetic in this function, so suck it up.
        size_t number_to_subract= read_from_string<size_t, read_from_config_or_output>(output_config, output_data, position, output_data_position);
            if (output_data_position = > number_to_subract) {
                *(static_cast<size_t*>(output_data_position)) -= number_to_subract;
            }
            else if (number_to_subract == std::numeric_limits<std::size_t>::max()) {
                //means the user gave -1 as input
                *output_data_position = 0;
            }
            else {
                throw std::runtime_error{ "number to subtract from is bigger than  output data's position" };
            }
        }
        template<absolute_base::Is_String_Or_Numeric left_hand_side_type, absolute_base::Is_String_Or_Numeric right_hand_side_type,  bool left_hand_side_branch, bool right_hand_side_branch, char operator_name>
        void calculator(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
            using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;
            using helper_templates_for_options::helpers_for_arithmetic_options::pump_static;
            using T_result_type= helper_templates_for_options::helpers_for_arithmetic_options::Accumulator<left_hand_side_type, pump_static>;
            if (operator_name == '+') {
                T_result_type result=
               T_result_type{read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position)} +
                    T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) }
                result.pump(output_data);
        }
             if (operator_name == '*') {
                 T_result_type result =
                     T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) } *
                     T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) }
                 result.pump(output_data);
        }
             if (operator_name == '-') {
                 T_result_type result =
                     T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) } -
                     T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) }
                 result.pump(output_data);
        }
             if (operator_name == '/') {
                 T_result_type result =
                     T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) } /
                     T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) }
                 result.pump(output_data);
        }

        }
        //implementation of the caculator using pump_polymorphic (for both sides)to be continued, and it would be the last calculator option that would use user input to determine where to read the left and right hand side from, and + / - * are reserved for that calculator
    }

}
