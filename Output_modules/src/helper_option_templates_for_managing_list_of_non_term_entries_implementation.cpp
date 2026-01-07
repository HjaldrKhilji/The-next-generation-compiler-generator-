module;
#include <string>
#include <string>       // For std::string, std::string::size_type
#include <algorithm>    // For std::find_if
#include <stdexcept>    // For std::runtime_error
#include <iterator>     // For std::distance (if needed) or iterator types
module Printer;

import All_declarations;
import Config_parser;
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
       return common_functions::read_number_from_string_at_a_position<int>(output_config, position);
        
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
read_number_from_string_at_a_position<int>(output_config, 
    find_sub_entry_index<find_nested_non_term_entry_using_number_or_name>
    {*entry, *output_config, position}
    );
    Siblings entry_in_sibling_state{ entry->sub_entries, sibling_index };
    int semantic_index= return_semantic_entry_helper<check_semantic_entry>
 (output_config, position, entry_in_sibling_state, list_of_entries_to_find_it_in);
    return indexes{ sibling_index, semantic_index, entry };
    }
}
