module;
#include <string>           // For std::string
#include <vector>           // For std::vector in semantic rules
#include <deque>            // Often used for "list_of_all_non_term_entries_for_fast_traversal"
#include <map>              // For map_for_fast_retrival_of_entries
#include <unordered_map>    // Alternative if using hash maps

#include <iterator>         // For std::distance and iterators
#include <iostream>         // For std::cout, std::cerr
#include <stdexcept>        // For std::runtime_error
#include <utility>          // For std::move
#include <functional>       // For std::reference_wrapper
#include <algorithm>        // For iterator arithmetic and algorithms
#include <limits>       // For std::numeric_limits<T>::max(), ::min(), ::lowest()

// I used AI to keep track of the headers needed


module all_declarations;
import estd_regex;
using absolute_base::Settings_for_semantical_rules;
template<typename config>
using absolute_base::All_non_terminal_entries_implementation;

template<typename config>
void All_non_terminal_entries::add_a_child_to_entry
(absolute_base::Non_terminal_name_entry<config> *entry_to_be_added_to,  absolute_base::Non_terminal_name_entry<config>&& entry_to_add)  {
    if (!map_for_fast_retrival_of_entries.contains(entry_to_be_added_to->name)) {
        throw std::string{ "COMPILER: entry not found" };
    }
    map_for_fast_retrival_of_entries[entry_to_be_added_to->name]->sub_entry.push_back(std::move(entry_to_add));
}
template<typename config>
void All_non_terminal_entries::add_semantic_rule_to_entry(absolute_base::Non_terminal_name_entry<config>* entry_to_be_added_to,  Semantical_analyzer_config_entry<config>&& semantical_rule_entry, int semantic_entry_index)  {
    auto& semantic_rules_for_entry = entry_to_be_added_to.all_semantical_analysis_rules;
    semantic_rules_for_entry.insert(semantic_rules_for_entry.begin() + semantic_entry_index, std::move(semantical_rule_entry));

}
template<typename config>
void All_non_terminal_entries::remove_entry
(absolute_base::Non_terminal_name_entry<config>* entry_to_remove)  {
    auto map_iterator = map_for_fast_retrival_of_entries.find(entry_to_remove->name);
    //it is the caller's(the whole projects) job to make sure that entry_to_remove is in the map
    

    map_for_fast_retrival_of_entries.erase(map_iterator);
    list_of_all_non_term_entries_for_fast_traversal.erase(map_iterator->second);


    
}
template<typename config>

void All_non_terminal_entries::remove_semantic_rule_of_entry(absolute_base::Non_terminal_name_entry<config>* entry_to_remove_from, int index_of_semantic_rule)  {
    auto& semantic_rules_for_entry = entry_to_remove_from.all_semantical_analysis_rules;
    semantic_rules_for_entry.erase(v.begin() + index_of_semantic_rule);
}

template<typename config>
void absolute_base::check_pattern(const std::vector < Semantical_analyzer_config_entry<config> >& semantical_checks, const std::string& text)  {
    std::string failed_matches{};
    uint64_t semantic_entry_index{};
    for (auto& x : semantical_checks) {
        uint64_t number_of_matches{};
        while(x.the_pattern_to_check.regex_match(text)){
            number_of_matches++;
        }
        if ((x.all_settings == Settings_for_semantical_rules::NONE) && number_of_matches) {
           failed_matches += "failed match index" + std::to_string(semantic_entry_index) + '\n';

        }
        else if (all_settings & Settings_for_semantical_rules::check_exist) {
            //the else if above had nested checks on all_settings, hence i use (bitiwsh) &(AND) instead of ==
            if (all_settings == Settings_for_semantical_rules::check_atleast) {
                if (x.maximum_amount_of_matches == std::numeric_limits<std::size_t>::max()) {
                    //means the useer entered -1 as maximum_amount_of_matches
                     //which means that he dosent want us to check the upper limit at all
                    if (number_of_matches > x.minimum_amount_of_matches) {
                       failed_matches += "failed match index" + std::to_string(semantic_entry_index) + '\n';
                    }
                }
                else {
                    if ((x.maximum_amount_of_matches < number_of_matches) && (number_of_matches > x.minimum_amount_of_matches)) {
                        failed_matches += "failed match index" + std::to_string(semantic_entry_index) + '\n';
                    }
                }
            }
            else if (all_settings == Settings_for_semantical_rules::check_exact)
            {

                if (number_of_matches != x.minimum_amount_of_matches) {
                   failed_matches += "failed match index" + std::to_string(semantic_entry_index) + '\n';
                }

            }
        }

        else if (all_settings == Settings_for_semantical_rules::check_dont_exist) {
            if (!number_of_matches) {
                failed_matches += "failed match index" + std::to_string(semantic_entry_index) + '\n';
            }
        }
        semantic_entry_index++;
    }
    if (failed_matches.length() != 0) {
        throw std::string{std::move(failed_matches)};
    }
    }

template<typename config>
void All_non_terminal_entries::print_all_content()  {
    //precondition:*this's  invariant is valid
    
    std::cout << "number of entries: " << list_of_all_non_term_entries_for_fast_traversal.size() << std::endl;
    for (const auto &current_entry : list_of_all_non_term_entries_for_fast_traversal) {
        std::cout << "current entry:" << std::endl;
        std::cout << current_entry.name << " ";
        std::cout << current_entry.pattern << " ";
        int index = 0;
        std::cout << "syntaxical data:" << std::endl;
        for (auto wrapped_sub_entries = current_entry.sub_entries.begin(); wrapped_sub_entries + index != current_entry.sub_entries.end(); ++index) {
            std::cout << "number of nested entries in current entry: " << current_entry.sub_entries.size() << std::endl;

            auto &unwrapped_sub_entries = wrapped_sub_entries[index].get();
            std::cout << unwrapped_sub_entries.name << unwrapped_sub_entries.pattern << " ";
            std::cout << "semantic data of every syntaxical data:" << std::endl;

            for (auto semantic_rules_in_sub_entries : current_entry.all_semantical_analysis_rules[index]) {
                std::cout << "number of semantic rules for current nested entry in current entry: " << current_entry.all_semantical_analysis_rules[index].size() << std::endl;

                std::cout << " " << semantic_rules_in_sub_entries.the_pattern_to_check << " " <<
                    semantic_rules_in_sub_entries.minimum_amount_of_matches << " " << semantic_rules_in_sub_entries.maximum_amount_of_matches << std::endl;;

            }

        }
        std::cout << "next entry(if any):" << std::endl;
    }
}
template<typename config>
void All_non_terminal_entries::add_non_term_symbol_name(uint64_tname)  {
  //precondition: "name" paremeter can not be empty
  //precondition: list_of_all_non_term_entries_for_fast_traversal is in a valid state 
  //postcondition: an entry is added to the "traversal list" only, hence you can only access the entry by traversing the deque,
  //the entry entered dose'nt contain anything other than the name.

    list_of_all_non_term_entries_for_fast_traversal.push_back(Non_terminal_name_entry {
      name,
      config{},
     Non_terminal_name_entry::sub_entry_type{},
      Non_terminal_name_entry::all_semantical_analysis_rules_type{}
    });

  }
template<typename config>
  void All_non_terminal_entries::add_non_term_pattern_for_newest_entry(config pattern)  {
    //precondition: "pattern" paremeter dose'nt contain a invalid regex expression
    //precondition: list_of_all_non_term_entries_for_fast_traversal is in a valid state and contains atleast a single entry
    //precondition: map_for_fast_retrival_of_entries is also in a valid state, and is amenable(mutable) enough to have a data inserted into it
    //postcondition: list_of_all_non_term_entries_for_fast_traversal gets the pattern for its latest entry entered
    //postcondition: map_for_fast_retrival_of_entries gets the "name" data member of the latest entry as key and the latest entry itself
    //entered as a entry into map_for_fast_retrival_of_entries
    auto newest_entry = list_of_all_non_term_entries_for_fast_traversal.end();
    newest_entry->pattern = pattern;
    //notice that this also a adds the entry to the map for fast retrival:
     map_for_fast_retrival_of_entries.insert({
      newest_entry.name,
      newest_entry
    });

  }
template<typename config>
  config& All_non_terminal_entries::get_pattern_of_nested_non_term_symbol_pattern(uint64_t sub_symbol_name)  {
  //precondition: "sub_symbol_name" paremeter can not be empty
  //precondition: map_for_fast_retrival_of_entries is in a valid state and is not empty
  //postcondition: the entry corrsponding to the key "sub_symbol_name" from map_for_fast_retrival_of_entries, and the copy of the 
  // "pattern" data member of that entry is returned

    if (!map_for_fast_retrival_of_entries.contains(sub_symbol_name)) {
      throw std::string("COMPILER: get_pattern_of_nested_non_term_symbol_pattern error");
    }

    return (map_for_fast_retrival_of_entries.at(sub_symbol_name))->pattern;

  }

template<typename config>
  void All_non_terminal_entries::add_nested_non_term_symbol_to_the_newest_entry(uint64_t sub_symbol_name)  {
    //precondition: "sub_symbol_name" paremeter can not be empty
    //precondition: map_for_fast_retrival_of_entries is in a valid state and is not empty
    //precondition: list_of_all_non_term_entries_for_fast_traversal is in a valid state and contains atleast a single entry
    //postcondition: the "sub_entries" field has a reference wrapper that points to a nested field appended to itself


      auto newest_entry = list_of_all_non_term_entries_for_fast_traversal.back();
      auto sub_entry_that_corrosponds_to_sub_name = map_for_fast_retrival_of_entries.at(sub_symbol_name);

    newest_entry.sub_entries.push_back(*sub_entry_that_corrosponds_to_sub_name);

  }
template<typename config>
  void All_non_terminal_entries::add_semantic_rule_for_newest_sub_entry<config>(const absolute_base::Semantical_analyzer_config_entry<config> && semantical_rule_entry)  {
    //I am using const rvalue reference to force the programmer to first gather all information, and then built the object using that in the same line as calling the function
    //hence avoiding any idiotic errors that he could do with such a important data structure, while trying to be "clever"
    //precondition: the paremeter "semantical_rule_entry" is in a valid state
    //precondition:list_of_all_non_term_entries_for_fast_traversal is in a valid state and has atleast one element
    //precondition:all_semantical_analysis_rules has  newest_entry.sub_entries.size() or newest_entry.sub_entries.size() -1 amount of element
    //postcondition: all_semantical_analysis_rules has a new semantic rule pushed to the newest element of itself
    //every element in "all_semantical_analysis_rules" has a semantic rule for the corrsponding element in "sub_entries" data member, and corrsponding 
    // in this specific sentence means the element with the same index.
    auto  newest_entry = list_of_all_non_term_entries_for_fast_traversal.back();;
    if (newest_entry.sub_entries.size() -1 == newest_entry.all_semantical_analysis_rules.size()) {
      //this means thats it the first semantic entry for the nested name
      newest_entry.all_semantical_analysis_rules.push_back(std::vector < Semantical_analyzer_config_entry<config> > {
        std::move(semantical_rule_entry)
      });
      return;
    }
    if (newest_entry.sub_entries.size()  == newest_entry.all_semantical_analysis_rules.size()) 
    {
    newest_entry.all_semantical_analysis_rules.back().push_back(  std::move(semantical_rule_entry));
    }
    else{
      throw std::string("COMPILER: add_semantic_rule_for_newest_sub_entry error");
    }
  }
template<typename config>
  absolute_base::Non_terminal_name_entry<config>& All_non_terminal_entries::get_current_non_term_entry(int index)  {
      //precondition:index is not negative and list_of_all_non_term_entries_for_fast_traversal is in a valid state
      //postcondition: returns a non term entry at the given index to the caller
      return list_of_all_non_term_entries_for_fast_traversal[index];

  }
template<typename config>
  absolute_base::Non_terminal_name_entry<config>& All_non_terminal_entries::get_current_nested_non_term_entry(int index_for_nested_non_term, int index)  {
      //precondition:the given indexes are not negative and list_of_all_non_term_entries_for_fast_traversal is in a valid state
    //postcondition: returns a nested non term entry at the index "[index][index_for_nested_non_term]"
      return *(list_of_all_non_term_entries_for_fast_traversal[index].sub_entries[index_for_nested_non_term]);

  }
