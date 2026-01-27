module;

#include <string>
#include <fstream>
#include <utility>
#include <span>
#include <limits>
#include <stdexcept>
#include <iostream>
// I used AI to keep track of the headers needed 

module Printer;
import Driver;
//this file contains options that dont read user input to find other options at runtime
namespace printing_tools {
    namespace options{
        //     ~functions to apply options on output data~


        std::string::size_type option_to_replicate_output(const std::string & output_config, std::string::size_type *position, std::string * output_data_to_append_to, std::string::size_type *output_data_position) {
            int number_of_times_to_replicate;
            try {
                number_of_times_to_replicate = absolute_base::read_number_from_string_at_a_position<int>(output_config, &position);
            }
            catch (std::string error_send_by_reader) {
                
                throw std::string{ "OUTPUT REPLICATION OPTION:", error_send_by_reader };
            }
            for (int i = 0; i < number_of_times_to_replicate; i++) {
                *output_data_to_append_to += *output_data_to_append_to;

            }
            return position + charactors_processed;

        }

        std::string::size_type option_to_change_output_stream(const std::string & output_config, std::string::size_type *position, std::string * output_data, std::string::size_type* output_data_position) {
            std::string file_name;
            try {
                file_name  = absolute_base::read_string_from_string_at_a_position(output_config, &position);
            }
            catch (std::string error_send_by_reader) {
                
                throw std::string{ "OPTION TO CHANGE OUTPUT STREAM:", error_send_by_reader };
            }
            catch (...) {
                throw std::string{ "OPTION TO CHANGE OUTPUT STREAM: totally unexpected error"};

            }
            output.switchToNewStream(new std::ofstream{ file_name });
            return delimiter_position + 1;//+1 is to skip the delemiter_position index itself
        }

        std::string::size_type option_to_change_input_stream(const std::string & output_config, std::string::size_type *position, std::string * output_data, std::string::size_type* output_data_position) {
            std::string file_name;
            try {
                file_name = absolute_base::read_string_from_string_at_a_position(output_config, &position);
            }
            catch (std::string error_send_by_reader) {
                
                throw std::string{ "OPTION TO CHANGE INPUT STREAM:", error_send_by_reader };
            }
            catch (...) {
                throw std::string{ "OPTION TO CHANGE INPUT STREAM: totally unexpected error" };

            }
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
            try {
                Non_terminal_name_entry* non_term_entry = helper_templates_for_options::return_non_terminal_entry<search>(output_config, position, list_of_entries_to_find_it_in);
                this->list_of_entries_to_find_it_in->remove_entry(non_term_entry);
            }
            catch (std::string error_sent_by_reporter) {

                throw std::string{ "OPTION TO REMOVE NON TERMINAL ENTRY: "+ error_sent_by_reporter};

            }
            catch (...) {
                throw std::string{ "OPTION TO REMOVE NON TERMINAL ENTRY: totally unexpected error" };

            }
        }
        template<bool search, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>
        void add_entry(const std::string & output_config, std::string::size_type * position, std::string * output_data, std::string::size_type* output_data_position) {
            try {
                Non_terminal_name_entry* non_term_entry = helper_templates_for_options::return_non_terminal_entry<search>(output_config, position, list_of_entries_to_find_it_in);
                config_parsing_tools::Config_reader_helper::push_latest_entry_as_sub_entry_of_an_entry<this->list_of_entries_to_find_it_in>(non_term_entry);
            }
            catch (std::string error_sent_by_reporter) {
                throw std::string{ "OPTION TO ADD NON TERMINAL ENTRY: " + error_sent_by_reporter };

            }
            catch (...) {
                throw std::string{ "OPTION TO ADD NON TERMINAL ENTRY: totally unexpected error" };

            }

        }

        template<bool find_parent_entry, bool find_nested_entry_technique, bool check_semantic_entry, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>
        void add_semantic_entry_to_non_term_entry_passed(const std::string & output_config, std::string::size_type * position, std::string * output_data, std::string::size_type* output_data_position) {
            try {
                indexes_and_non_term_entry info_needed = helper_templates_for_options::return_semantic_entry
                    <find_parent_entry, find_nested_entry_technique, check_semantic_entry>
                    (output_config, position, this->list_of_entries_to_find_it_in);
                std::span<char> sub_span{ output_config.data() + *position, output_config.size() - *position) };

                Semantical_analyzer_config_entry semantic_entry =
                    config_parsing_tools::Config_reader_helper::return_semantical_analyzer_entry(
                        sub_span, this->list_of_entries_to_find_it_in
                    );
                this->list_of_entries_to_find_it_in->add_semantic_rule_to_entry(info_needed.non_term_entry, std::move(semantic_entry), info_needed.sibling_index, info_needed.semantic_entry_index);
            }
            catch (std::string error_sent_by_reporter) {
                throw std::string{ "OPTION TO ADD SEMANTIC ENTRY TO THE NON TERMINAL ENTRY PASSED: " + error_sent_by_reporter };

            }
            catch (...) {
                throw std::string{ "OPTION TO ADD SEMANTIC ENTRY TO THE NON TERMINAL ENTRY PASSED:  totally unexpected error" };

            }
        }
        template<bool find_parent_entry, bool find_nested_entry_technique, bool check_semantic_entry, absolute_base::All_non_terminal_entries Printer::* list_of_entries_to_find_it_in>

        void remove_semantic_entry_to_non_term_entry_passed(const std::string& output_config, std::string::size_type* position, std::string* output_data, , std::string::size_type* output_data_position) {
            try {
                indexes_and_non_term_entry info_needed = helper_templates_for_options::return_semantic_entry
                    <find_parent_entry, find_nested_entry_technique, check_semantic_entry>
                    (output_config, position, this->list_of_entries_to_find_it_in);
                this->list_of_entries_to_find_it_in->remove_latest_semantic_rule_for_entry(info_needed.non_term_entry, info_needed.sibling_index, info_needed.semantic_entry_index);
            }
            catch (std::string error_sent_by_reporter) {
                throw std::string{ "OPTION TO REMOVE SEMANTIC ENTRY FROM THE NON TERMINAL ENTRY PASSED: " + error_sent_by_reporter };

            }
            catch (...) {
                throw std::string{ "OPTION TO REMOVE SEMANTIC ENTRY FROM THE NON TERMINAL ENTRY PASSED:  totally unexpected error" };

            }
        }
        void trim_output_from_current_position_to_end(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
            output_data->erase(*output_data_position);
        }

        template<bool read_from_config_or_output>
        void subtract_from_output_data_position(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
        try {
            using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;

            //a function from helper_templates_for_options::helpers_for_arithmetic_options is ideally not to be used here, but I had to since I didnt know a better place to put this function in
            //in my defense it is okay, since I am doing some arithmetic in this function, so suck it up.
            size_t number_to_subract = read_from_string<size_t, read_from_config_or_output>(output_config, output_data, position, output_data_position);
            if (output_data_position = > number_to_subract) {
                *(static_cast<uint64_t*>(output_data_position)) -= number_to_subract;
            }
            else if (number_to_subract == std::numeric_limits<std::size_t>::max()) {
                //means the user gave -1 as input
                *output_data_position = 0;
            }
            else {
                //its a compiler error by defintion
                throw std::string{ "COMPILER: number to subtract is bigger than current position" };
            }
        }
        catch (std::string error_sent_by_reporter) {
            throw std::string{ "OPTION TO SUBTRACT FROM OUTPUT DATA POSITION PASSED: " + error_sent_by_reporter };

        }
        catch (...) {
            throw std::string{ "OPTION TO SUBTRACT FROM OUTPUT DATA POSITION PASSED:  totally unexpected error" };

        }

        }
        template<absolute_base::Is_String_Or_Numeric left_hand_side_type, absolute_base::Is_String_Or_Numeric right_hand_side_type,  bool left_hand_side_branch, bool right_hand_side_branch, char operator_name>
        void calculator(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
            try {
                using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;


                using T_result_type = helper_templates_for_options::helpers_for_arithmetic_options::Accumulator<left_hand_side_type>;
                constexpr if (operator_name == '+') {
                    T_result_type result =
                        T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) } +
                        T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) }
                    result.pump(output_data);
                }
                else if (operator_name == '*') {
                    T_result_type result =
                        T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) } *
                        T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) }
                    result.pump(output_data);
                }
                else if (operator_name == '-') {
                    T_result_type result =
                        T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) } -
                        T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) }
                    result.pump(output_data);
                }
                else if (operator_name == '/') {
                    T_result_type result =
                        T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) } /
                        T_result_type{ read_from_string<left_hand_side_type, left_hand_side_branch >(output_config, output_data, position, output_data_position) }
                    result.pump(output_data);
                }
                else {
                    static_assert(1, "wrong operator");
                }
            }
            catch(std::string error_sent_by_reader)
            {
                throw std::string{ "STATIC ARETHIMETIC ENGINE: " + error_sent_by_reader };
            }
            catch (...) {
                throw std::string{ "STATIC ARETHIMETIC ENGINE:  totally unexpected error" };

            }

        }
        template<char operator_name>
        void polymorphic_calculator(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
            using helper_templates_for_options::helpers_for_arithmetic_options::read_polymorphically_from_string;
            using T_result_type = helper_templates_for_options::helpers_for_arithmetic_options::Accumulator<Polymorphic_accumulator>;
            try {
                constexpr if (operator_name == '+') {
                    T_result_type result =
                        T_result_type{ read_polymorphically_from_string(output_config, position) }
                    + T_result_type{ read_polymorphically_from_string(output_config, position) };
                    result.pump(output_data);

                }
                else if (operator_name == '-') {
                    T_result_type result =
                        T_result_type{ read_polymorphically_from_string(output_config, position) }
                    - T_result_type{ read_polymorphically_from_string(output_config, position) };
                    result.pump(output_data);

                }
                else if (operator_name == '*') {
                    T_result_type result =
                        T_result_type{ read_polymorphically_from_string(output_config, position) }
                    - T_result_type{ read_polymorphically_from_string(output_config, position) };
                    result.pump(output_data);

                }
                else if (operator_name == '/') {
                    T_result_type result =
                        T_result_type{ read_polymorphically_from_string(output_config, position) }
                    / T_result_type{ read_polymorphically_from_string(output_config, position) };
                    result.pump(output_data);

                }
                else if (operator_name == '|') {
                    T_result_type result =
                        T_result_type{ read_polymorphically_from_string(output_config, position) }
                    | T_result_type{ read_polymorphically_from_string(output_config, position) };
                    result.pump(output_data);

                }
                else if (operator_name == '&') {
                    T_result_type result =
                        T_result_type{ read_polymorphically_from_string(output_config, position) }
                    &T_result_type{ read_polymorphically_from_string(output_config, position) };
                    result.pump(output_data);

                }
                else if (operator_name == '^') {
                    T_result_type result =
                        T_result_type{ read_polymorphically_from_string(output_config, position) }
                    ^ T_result_type{ read_polymorphically_from_string(output_config, position) };
                    result.pump(output_data);

                }
            } 
            catch (std::string engine_error_label) {
                throw std::string{ engine_error_label + ":a string used as an operand to the " + operator_name + " operator"}

            }
            catch (...) {
                throw std::string{ "ONE OF DYNAMIC ARETHIMETIC OPTIONS PASSED:  totally unexpected error. ERROR NOT CAUGHT BY THE RESPECTIVE ENGINE" };

            }

        }
        template<bool source_is_output_config_or_output_data>
        void start_nested(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
            using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;

            driver::Driver_engine engine = driver::create_driver(
                read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position),
                read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position),
                read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position),
                read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position),
                read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position),
                read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position),
                absolute_base::convert_to_bool(read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position)),
                absolute_base::convert_to_bool(read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position)),
                absolute_base::convert_to_bool(read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position))

                );
            
            engine.run_engine();
        }
        
        template<bool source_is_output_config_or_output_data, bool get_from_ordered_or_else_hashed>
        void add_data_to_output_config(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
            //can be used to make functions
            using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;
            try {
                using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;
                std::string variable_name = read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position);
                constexpr if (get_from_ordered_or_else_hashed) {
                    auto value = all_variable_ordered_storage.at(variable_name);
                    value.pump(const_cast<std::string&>(output_config));
                }
                else {
                    auto value = all_variable_hashed_storage.at(variable_name);
                    value.pump(const_cast<std::string&>(output_config));

                }
            }

            catch (std::string error_sent_by_reader)
            {
                throw std::string{ "POLYMORPHIC VARIABLE GET: " + error_sent_by_reader };
            }
            catch (...) {
                throw std::string{ "POLYMORPHIC VARIABLE GET:  totally unexpected error" };

            }
            
        }
       
        std::map<std::string, printing_tools::helper_templates_for_options::helpers_for_arithmetic_options::Polymorphic_accumulator> all_variable_ordered_storage{};
        std::unordered_map<std::string, printing_tools::helper_templates_for_options::helpers_for_arithmetic_options::Polymorphic_accumulator> all_variable_hashed_storage{};
        
        template<bool source_is_output_config_or_output_data, bool get_from_ordered_or_else_hashed>
        void store_variable(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
            try {
                using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;
                std::string variable_name = read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position);
                constexpr if (get_from_ordered_or_else_hashed) {
                    all_variable_ordered_storage[variable_name]= helper_templates_for_options::helpers_for_arithmetic_options::read_polymorphically_from_string
                        <source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position);
                }
                else {
                    all_variable_hashed_storage[variable_name] = helper_templates_for_options::helpers_for_arithmetic_options::read_polymorphically_from_string
                        <source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position);

                }
            }
            
            catch (std::string error_sent_by_reader)
            {
                throw std::string{ "POLYMORPHIC VARIABLE STORAGE: " + error_sent_by_reader };
            }
            catch (...) {
                throw std::string{ "POLYMORPHIC VARIABLE STORAGE:  totally unexpected error" };

            }
        
        }

        template<bool source_is_output_config_or_output_data, bool get_from_ordered_or_else_hashed>
        void get_polymorphic(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
            try {
                using helper_templates_for_options::helpers_for_arithmetic_options::read_from_string;
                std::string variable_name = read_from_string<std::string, source_is_output_config_or_output_data>(output_config, output_data, position, output_data_position);
                constexpr if (get_from_ordered_or_else_hashed) {
                    auto value = all_variable_ordered_storage.at(variable_name);
                    value.pump(output_data);
                }
                else {
                    auto value = all_variable_hashed_storage.at(variable_name);
                     value.pump(output_data);

                }
            }

            catch (std::string error_sent_by_reader)
            {
                throw std::string{ "POLYMORPHIC VARIABLE GET: " + error_sent_by_reader };
            }
            catch (...) {
                throw std::string{ "POLYMORPHIC VARIABLE GET:  totally unexpected error" };

            }

        }

        }

}
template<bool source_is_output_config_or_output_data, bool get_from_ordered_or_else_hashed>
void store_in_cache(const std::string& output_config, std::string::size_type* position, std::string* output_data, std::string::size_type* output_data_position) {
    std::string cache_name = read_from_string<std::string>(output_config, position);

    constexpr if (get_from_ordered_or_else_hashed) {
        constexpr if (source_is_output_config_or_output_data) {
            all_variable_ordered_storage[variable_name] = output_config;
        }
        else {
            all_variable_hashed_storage[variable_name] = output_data_position;

        }
    }
    else {
        constexpr if (source_is_output_config_or_output_data) {
            all_variable_ordered_storage[variable_name] = output_config;
        }
        else {
            all_variable_hashed_storage[variable_name] = output_data_position;

        }
    }
}
