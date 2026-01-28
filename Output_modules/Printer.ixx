module;
// For string manipulation and positions
#include <string>

#include<concepts>

#include<type_traits>

#include<future>

#include <spanstream> 

#include <array>          
// For the fixed-size dispatch table
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
        using Option_functions_wrapper_type = std::string::size_type(Printer::*)(const std::string&, std::string::size_type*, std::string*, std::string::size_type*);

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

            for (char option_charactor : output_config_entry.output_config_data) {
             
                        //output_config_entry.output_config_data should never be changed to a local const variable (EVER!!!!)
                        //dont move output_config_entry.output_config_data as well into a local variable as well, you can copy but that would be expensive.
            operations_dispatch_table[option_charactor](output_config_entry.output_config_data, &position, &string_to_output, &output_data_position);
            ++position;//skipping the option charactor found
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
        Printer(Output_stream_handler_ptr a, std::unique_ptr<absolute_base::All_non_terminal_entries> b, std::shared_ptr<absolute_base::All_non_terminal_entries> c, Input_stream_handler_ptr  d, std::shared_ptr<bool> e) : output{ a }, all_config_for_output{ b }, all_config_for_input{ c }, input{ d }, multithreaded{ e }
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

        std::shared_ptr<bool> multithreaded;


        // The complete, fixed-size instruction set (AI generated(i wouldnt write all this myself) (256 entries)
static const Option_functions_wrapper_type operations_dispatch_table[256] = {
    /* 0x00 - 0x05: UTILITY */
    &options::print_output,                         /* 0x00 */
    &options::option_to_replicate_output,           /* 0x01 */
    &options::option_to_change_output_stream,       /* 0x02 */
    &options::option_to_change_input_stream,        /* 0x03 */
    &options::option_to_hash,                       /* 0x04 */
    &options::trim_output_from_current_position_to_end, /* 0x05 */

    /* 0x06 - 0x0F: RESERVED */
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op,

    /* 0x10 - 0x17: STRUCTURAL */
    &options::subtract_from_output_data_position<true>,  /* 0x10 */
    &options::subtract_from_output_data_position<false>, /* 0x11 */
    &options::start_nested<true>,                        /* 0x12 */
    &options::start_nested<false>,                       /* 0x13 */
    &options::add_entry<true,  &Printer::all_config_for_input>,    /* 0x14 */
    &options::add_entry<false, &Printer::all_config_for_input>,    /* 0x15 */
    &options::remove_entry<true,  &Printer::all_config_for_input>, /* 0x16 */
    &options::remove_entry<false, &Printer::all_config_for_input>, /* 0x17 */

    /* 0x18 - 0x1B: RESERVED */
    &options::no_op, &options::no_op, &options::no_op, &options::no_op

    /* 0x1C - 0x2F: FLOW CONTROL AND VARIABLES*/
    &options:remove_polymorphic<true, true> //0x1C
    &options:remove_polymorphic<true, false> //0x1D
    &options:remove_polymorphic<false, true> //0x1E
    &options:remove_polymorphic<false, false> //0x1F
    &options::loop<true,  true>,   &options::loop<true,  false>,   /* 0x20, 0x21 */
    &options::loop<false, true>,   &options::loop<false, false>,   /* 0x22, 0x23 */
    &options::branch<true,  true>, &options::branch<true,  false>, /* 0x24, 0x25 */
    &options::branch<false, true>, &options::branch<false, false>, /* 0x26, 0x27 */
    &options::store_variable<true,  true>,  &options::store_variable<true,  false>,  /* 0x28, 0x29 */
    &options::store_variable<false, true>,  &options::store_variable<false, false>,  /* 0x2A, 0x2B */
    &options::get_polymorphic<true,  true>, &options::get_polymorphic<true,  false>, /* 0x2C, 0x2D */
    &options::get_polymorphic<false, true>, &options::get_polymorphic<false, false>, /* 0x2E, 0x2F */

    /* 0x30 - 0x33: CONFIG DATA */
    &options::add_data_to_output_config<true,  true>,  /* 0x30 */
    &options::add_data_to_output_config<true,  false>, /* 0x31 */
    &options::add_data_to_output_config<false, true>,  /* 0x32 */
    &options::add_data_to_output_config<false, false>, /* 0x33 */

    /* 0x34 - 0x3F: RESERVED */
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,

    /* 0x40 - 0x4F: SEMANTIC ENTRIES */
    &options::add_semantic_entry_to_non_term_entry_passed<true,  true,  true,  &Printer::all_config_for_input>, /* 0x40 */
    &options::add_semantic_entry_to_non_term_entry_passed<true,  true,  false, &Printer::all_config_for_input>, /* 0x41 */
    &options::add_semantic_entry_to_non_term_entry_passed<true,  false, true,  &Printer::all_config_for_input>, /* 0x42 */
    &options::add_semantic_entry_to_non_term_entry_passed<true,  false, false, &Printer::all_config_for_input>, /* 0x43 */
    &options::add_semantic_entry_to_non_term_entry_passed<false, true,  true,  &Printer::all_config_for_input>, /* 0x44 */
    &options::add_semantic_entry_to_non_term_entry_passed<false, true,  false, &Printer::all_config_for_input>, /* 0x45 */
    &options::add_semantic_entry_to_non_term_entry_passed<false, false, true,  &Printer::all_config_for_input>, /* 0x46 */
    &options::add_semantic_entry_to_non_term_entry_passed<false, false, false, &Printer::all_config_for_input>, /* 0x47 */
    &options::remove_semantic_entry_to_non_term_entry_passed<true,  true,  true,  &Printer::all_config_for_input>, /* 0x48 */
    &options::remove_semantic_entry_to_non_term_entry_passed<true,  true,  false, &Printer::all_config_for_input>, /* 0x49 */
    &options::remove_semantic_entry_to_non_term_entry_passed<true,  false, true,  &Printer::all_config_for_input>, /* 0x4A */
    &options::remove_semantic_entry_to_non_term_entry_passed<true,  false, false, &Printer::all_config_for_input>, /* 0x4B */
    &options::remove_semantic_entry_to_non_term_entry_passed<false, true,  true,  &Printer::all_config_for_input>, /* 0x4C */
    &options::remove_semantic_entry_to_non_term_entry_passed<false, true,  false, &Printer::all_config_for_input>, /* 0x4D */
    &options::remove_semantic_entry_to_non_term_entry_passed<false, false, true,  &Printer::all_config_for_input>, /* 0x4E */
    &options::remove_semantic_entry_to_non_term_entry_passed<false, false, false, &Printer::all_config_for_input>, /* 0x4F */

    /* 0x50 - 0x56: POLYMORPHIC MATH */
    &options::polymorphic_calculator<'+'>, &options::polymorphic_calculator<'-'>, /* 0x50, 0x51 */
    &options::polymorphic_calculator<'*'>, &options::polymorphic_calculator<'/'>, /* 0x52, 0x53 */
    &options::polymorphic_calculator<'|'>, &options::polymorphic_calculator<'&'>, /* 0x54, 0x55 */
    &options::polymorphic_calculator<'^'>,                                        /* 0x56 */

    /* 0x57 - 0x5F: RESERVED */
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op, &options::no_op,

    /* 0x60 - 0x83: CALCULATOR (ADDITION) */
    &options::calculator<long long,   long double, true,  true,  '+'>, /* 0x60 */
    &options::calculator<long long,   long double, true,  false, '+'>, /* 0x61 */
    &options::calculator<long long,   long double, false, true,  '+'>, /* 0x62 */
    &options::calculator<long long,   long double, false, false, '+'>, /* 0x63 */
    &options::calculator<long double, long long,   true,  true,  '+'>, /* 0x64 */
    &options::calculator<long double, long long,   true,  false, '+'>, /* 0x65 */
    &options::calculator<long double, long long,   false, true,  '+'>, /* 0x66 */
    &options::calculator<long double, long long,   false, false, '+'>, /* 0x67 */
    &options::calculator<std::string, long long,   true,  true,  '+'>, /* 0x68 */
    &options::calculator<std::string, long long,   true,  false, '+'>, /* 0x69 */
    &options::calculator<std::string, long long,   false, true,  '+'>, /* 0x6A */
    &options::calculator<std::string, long long,   false, false, '+'>, /* 0x6B */
    &options::calculator<long long,   std::string, true,  true,  '+'>, /* 0x6C */
    &options::calculator<long long,   std::string, true,  false, '+'>, /* 0x6D */
    &options::calculator<long long,   std::string, false, true,  '+'>, /* 0x6E */
    &options::calculator<long long,   std::string, false, false, '+'>, /* 0x6F */
    &options::calculator<std::string, long double, true,  true,  '+'>, /* 0x70 */
    &options::calculator<std::string, long double, true,  false, '+'>, /* 0x71 */
    &options::calculator<std::string, long double, false, true,  '+'>, /* 0x72 */
    &options::calculator<std::string, long double, false, false, '+'>, /* 0x73 */
    &options::calculator<long double, std::string, true,  true,  '+'>, /* 0x74 */
    &options::calculator<long double, std::string, true,  false, '+'>, /* 0x75 */
    &options::calculator<long double, std::string, false, true,  '+'>, /* 0x76 */
    &options::calculator<long double, std::string, false, false, '+'>, /* 0x77 */
    &options::calculator<long long,   long long,   true,  true,  '+'>, /* 0x78 */
    &options::calculator<long long,   long long,   true,  false, '+'>, /* 0x79 */
    &options::calculator<long long,   long long,   false, true,  '+'>, /* 0x7A */
    &options::calculator<long long,   long long,   false, false, '+'>, /* 0x7B */
    &options::calculator<long double, long double, true,  true,  '+'>, /* 0x7C */
    &options::calculator<long double, long double, true,  false, '+'>, /* 0x7D */
    &options::calculator<long double, long double, false, true,  '+'>, /* 0x7E */
    &options::calculator<long double, long double, false, false, '+'>, /* 0x7F */
    &options::calculator<std::string, std::string, true,  true,  '+'>, /* 0x80 */
    &options::calculator<std::string, std::string, true,  false, '+'>, /* 0x81 */
    &options::calculator<std::string, std::string, false, true,  '+'>, /* 0x82 */
    &options::calculator<std::string, std::string, false, false, '+'>, /* 0x83 */

    /* 0x84 - 0x8F: RESERVED */
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,

    /* 0x90 - 0x97: CALCULATOR (SUBTRACTION) */
    &options::calculator<long long,   long double, true,  true,  '-'>, /* 0x90 */
    &options::calculator<long long,   long double, false, false, '-'>, /* 0x91 */
    &options::calculator<long double, long long,   true,  true,  '-'>, /* 0x92 */
    &options::calculator<long double, long long,   false, false, '-'>, /* 0x93 */
    &options::calculator<long long,   long long,   true,  true,  '-'>, /* 0x94 */
    &options::calculator<long long,   long long,   false, false, '-'>, /* 0x95 */
    &options::calculator<long double, long double, true,  true,  '-'>, /* 0x96 */
    &options::calculator<long double, long double, false, false, '-'>, /* 0x97 */

    /* 0x98 - 0x9F: RESERVED */
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,

    /* 0xA0 - 0xA7: CALCULATOR (MULTIPLICATION) */
    &options::calculator<long long,   long double, true,  true,  '*'>, /* 0xA0 */
    &options::calculator<long long,   long double, false, false, '*'>, /* 0xA1 */
    &options::calculator<long double, long long,   true,  true,  '*'>, /* 0xA2 */
    &options::calculator<long double, long long,   false, false, '*'>, /* 0xA3 */
    &options::calculator<long long,   long long,   true,  true,  '*'>, /* 0xA4 */
    &options::calculator<long long,   long long,   false, false, '*'>, /* 0xA5 */
    &options::calculator<long double, long double, true,  true,  '*'>, /* 0xA6 */
    &options::calculator<long double, long double, false, false, '*'>, /* 0xA7 */

    /* 0xA8 - 0xAF: RESERVED */
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,

    /* 0xB0 - 0xB7: CALCULATOR (DIVISION) */
    &options::calculator<long long,   long double, true,  true,  '/'>, /* 0xB0 */
    &options::calculator<long long,   long double, false, false, '/'>, /* 0xB1 */
    &options::calculator<long double, long long,   true,  true,  '/'>, /* 0xB2 */
    &options::calculator<long double, long long,   false, false, '/'>, /* 0xB3 */
    &options::calculator<long long,   long long,   true,  true,  '/'>, /* 0xB4 */
    &options::calculator<long long,   long long,   false, false, '/'>, /* 0xB5 */
    &options::calculator<long double, long double, true,  true,  '/'>, /* 0xB6 */
    &options::calculator<long double, long double, false, false, '/'>, /* 0xB7 */

    /* 0xB8 - 0xBF: CACHE OPTIONS */
    &options::store_in_cache<true,  true>,  /* 0xB8 */
    &options::store_in_cache<false, false>, /* 0xB9 */
    &options::store_in_cache<true,  false>, /* 0xBA */
    &options::store_in_cache<false, true>,  /* 0xBB */
    &options::get_from_cache<true,  true>,  /* 0xBC */
    &options::get_from_cache<false, false>, /* 0xBD */
    &options::get_from_cache<true,  false>, /* 0xBE */
    &options::get_from_cache<false, true>,  /* 0xBF */

    /* 0xC0 - 0xC9: RESERVED */
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op,

    /* 0xCA - 0xCB: MULTITHREADING FLAGS */
    &options::change_value_of_bool_owned_by_shared_ptr<&Printer::multithreaded, true>,  /* 0xCA */
    &options::change_value_of_bool_owned_by_shared_ptr<&Printer::multithreaded, false>, /* 0xCB */

    /* 0xCC - 0xFF: RESERVED (52 slots) */
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op,
    &options::no_op, &options::no_op, &options::no_op, &options::no_op
};



    };

};
