module;
#include <iostream>


#include <map>


#include <sstream>

#include <string>

#include <vector>
#include<regex>
#include<deque>

module all_declarations;
using Iterator_for_list_of_entries = absolute_base::All_non_terminal_entries_implementation::Iterator_for_list_of_entries;
using absolute_base::Settings_for_semantical_rules;
using absolute_base::All_non_terminal_entries_implementation;

void All_non_terminal_entries_implementation::add_a_child_to_entry
(absolute_base::Non_terminal_name_entry *entry_to_be_added_to,  absolute_base::Non_terminal_name_entry&& entry_to_add) override{
    if (!map_for_fast_retrival_of_entries.contains(entry_to_be_added_to->name)) {
        throw std::runtime_error{ "entry not found" };
    }
    map_for_fast_retrival_of_entries[entry_to_be_added_to->name]->sub_entry.push_back(std::move(entry_to_add));
}
void All_non_terminal_entries_implementation::add_semantic_rule_to_entry(absolute_base::Non_terminal_name_entry* entry_to_be_added_to,  Semantical_analyzer_config_entry&& semantical_rule_entry, int sibling_index) override {
    auto& semantic_rules_for_entry = entry_to_remove.all_semantical_analysis_rules[sibling_index];
    semantic_rules_for_entry.push_back(std::move(semantical_rule_entry));
}
void All_non_terminal_entries_implementation::remove_entry
(absolute_base::Non_terminal_name_entry* entry_to_remove) override {
    Iterator_for_list_of_entries map_iterator = map_for_fast_retrival_of_entries.find(entry_to_remove->name);
    //it is the caller's(the whole projects) job to make sure that entry_to_remove is in the map
    

    map_for_fast_retrival_of_entries.erase(map_iterator);
    list_of_all_non_term_entries_for_fast_traversal.erase(map_iterator->second);


    
}

void All_non_terminal_entries_implementation::remove_latest_semantic_rule_for_entry(absolute_base::Non_terminal_name_entry* entry_to_remove, int sibling_index) override {
    auto& semantic_rules_for_entry = entry_to_remove.all_semantical_analysis_rules[sibling_index];
    semantic_rules_for_entry.pop_back();  
}

void All_non_terminal_entries_implementation::remove_all_semantic_rules_for_entry(absolute_base::Non_terminal_name_entry* entry_to_remove,const std::string pattern_to_remove_all_occurances_of, int sibling_index) override {
    auto& semantic_rules_for_entry = entry_to_remove.all_semantical_analysis_rules[sibling_index];
    words.erase(std::remove(vector_to_remove_all_occurances_in.begin(), vector_to_remove_all_occurances_in.end(), pattern_to_remove_all_occurances_of), vector_to_remove_all_occurances_in.end());
}
bool absolute_base::Semantical_analyzer_config_entry::check_pattern(std::string text) {
    std::regex pattern{ the_pattern_to_check };
    
        int number_of_matches = static_cast<size_t>(std::distance(
            std::sregex_iterator(text.begin(), text.end(), pattern),
            std::sregex_iterator() // Default constructor represents the end-of-sequence
        ));
        if (all_settings == Settings_for_semantical_rules::NONE) {
            return true;
        }
        if (all_settings && Settings_for_semantical_rules::check_exist) {
            if (all_settings && Settings_for_semantical_rules::check_atleast) {
                if (number_of_matches == minimum_amount_of_matches) {
                    return true;
                }
                else {
                    return false;
                }
            }
            if (all_settings && Settings_for_semantical_rules::check_exact)
            {
                if ((maximum_amount_of_matches <number_of_matches) && (number_of_matches> minimum_amount_of_matches)) {
                    return false;
                }
                else {
                    return false;
                }


            }
        }
        if (all_settings && Settings_for_semantical_rules::check_dont_exist) {
            if (number_of_matches == 0) {
                return true;
            }
            else {
                return false;
            }
        }

    }


void All_non_terminal_entries_implementation::print_all_content() {
    //precondition:*this's  invariant is valid
    
    std::cout << "number of entries: " << list_of_all_non_term_entries_for_fast_traversal.size() << std::endl;
    for (const absolute_base::Non_terminal_name_entry &current_entry : list_of_all_non_term_entries_for_fast_traversal) {
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

                std::cout << semantic_rules_in_sub_entries.name_of_non_term_symbol_to_check.get() << " " << semantic_rules_in_sub_entries.the_pattern_to_check << " " <<
                    semantic_rules_in_sub_entries.minimum_amount_of_matches << " " << semantic_rules_in_sub_entries.maximum_amount_of_matches << std::endl;;

            }

        }
        std::cout << "next entry(if any):" << std::endl;
    }
}

void All_non_terminal_entries_implementation::add_non_term_symbol_name(std::string name) {
  //precondition: "name" paremeter can not be empty
  //precondition: list_of_all_non_term_entries_for_fast_traversal is in a valid state 
  //postcondition: an entry is added to the "traversal list" only, hence you can only access the entry by traversing the deque,
  //the entry entered dose'nt contain anything other than the name.

    list_of_all_non_term_entries_for_fast_traversal.push_back(Non_terminal_name_entry {
      name,
      "",
     Non_terminal_name_entry::sub_entry_type{},
      Non_terminal_name_entry::all_semantical_analysis_rules_type{}
    });

  }
  void All_non_terminal_entries_implementation::add_non_term_pattern_for_newest_entry(std::string pattern) {
    //precondition: "pattern" paremeter dose'nt contain a invalid regex expression
    //precondition: list_of_all_non_term_entries_for_fast_traversal is in a valid state and contains atleast a single entry
    //precondition: map_for_fast_retrival_of_entries is also in a valid state, and is amenable(mutable) enough to have a data inserted into it
    //postcondition: list_of_all_non_term_entries_for_fast_traversal gets the pattern for its latest entry entered
    //postcondition: map_for_fast_retrival_of_entries gets the "name" data member of the latest entry as key and the latest entry itself
    //entered as a entry into map_for_fast_retrival_of_entries
    Iterator_for_list_of_entries& newest_entry = list_of_all_non_term_entries_for_fast_traversal.end();
    newest_entry->pattern = pattern;
    //notice that this also a adds the entry to the map for fast retrival:
     map_for_fast_retrival_of_entries.insert({
      newest_entry.name,
      newest_entry
    });

  }
  std::string & All_non_terminal_entries_implementation::get_pattern_of_nested_non_term_symbol_pattern(std::string sub_symbol_name) {
  //precondition: "sub_symbol_name" paremeter can not be empty
  //precondition: map_for_fast_retrival_of_entries is in a valid state and is not empty
  //postcondition: the entry corrsponding to the key "sub_symbol_name" from map_for_fast_retrival_of_entries, and the copy of the 
  // "pattern" data member of that entry is returned

    if (!map_for_fast_retrival_of_entries.contains(sub_symbol_name)) {
      std::cerr<<"no pattern founds"<<std::endl;
      throw std::runtime_error("get_pattern_of_nested_non_term_symbol_pattern error");
    }

    return (map_for_fast_retrival_of_entries.at(sub_symbol_name))->pattern;

  }
  std::reference_wrapper < std::string > All_non_terminal_entries_implementation::get_parmenant_name_of_nested_non_term_symbol_pattern(std::string sub_symbol_name) {
//precondition: "sub_symbol_name" paremeter can not be empty
  //precondition: map_for_fast_retrival_of_entries is in a valid state and is not empty
  //postcondition: the entry corrsponding to the key "sub_symbol_name" from map_for_fast_retrival_of_entries, and the reference wrapper containing
  //a reference to the data member "pattern" of the entry is returned
    if (!map_for_fast_retrival_of_entries.contains(sub_symbol_name)) {
      std::cerr<<"no pattern found"<<std::endl;
      throw std::runtime_error("get_parmenant_name_of_nested_non_term_symbol_pattern error");

    }

    return map_for_fast_retrival_of_entries.at(sub_symbol_name)->name;

  }
  void All_non_terminal_entries_implementation::add_nested_non_term_symbol_to_the_newest_entry(std::string sub_symbol_name) {
    //precondition: "sub_symbol_name" paremeter can not be empty
    //precondition: map_for_fast_retrival_of_entries is in a valid state and is not empty
    //precondition: list_of_all_non_term_entries_for_fast_traversal is in a valid state and contains atleast a single entry
    //postcondition: the "sub_entries" field has a reference wrapper that points to a nested field appended to itself


      Non_terminal_name_entry newest_entry& = list_of_all_non_term_entries_for_fast_traversal.back();
      Iterator_for_list_of_entries sub_entry_that_corrosponds_to_sub_name = map_for_fast_retrival_of_entries.at(sub_symbol_name);

    newest_entry.sub_entries.push_back(*sub_entry_that_corrosponds_to_sub_name);

  }

  void All_non_terminal_entries_implementation::add_semantic_rule_for_newest_sub_entry(const absolute_base::Semantical_analyzer_config_entry && semantical_rule_entry) {
    //I am using const rvalue reference to force the programmer to first gather all information, and then built the object using that in the same line as calling the function
    //hence avoiding any idiotic errors that he could do with such a important data structure, while trying to be "clever"
    //precondition: the paremeter "semantical_rule_entry" is in a valid state
    //precondition:list_of_all_non_term_entries_for_fast_traversal is in a valid state and has atleast one element
    //precondition:all_semantical_analysis_rules has  newest_entry.sub_entries.size() or newest_entry.sub_entries.size() -1 amount of element
    //postcondition: all_semantical_analysis_rules has a new semantic rule pushed to the newest element of itself
    //every element in "all_semantical_analysis_rules" has a semantic rule for the corrsponding element in "sub_entries" data member, and corrsponding 
    // in this specific sentence means the element with the same index.
    auto & newest_entry = list_of_all_non_term_entries_for_fast_traversal.back();;
    if (newest_entry.sub_entries.size() -1 == newest_entry.all_semantical_analysis_rules.size()) {
      //this means thats it the first semantic entry for the nested name
      newest_entry.all_semantical_analysis_rules.push_back(std::vector < Semantical_analyzer_config_entry > {
        std::move(semantical_rule_entry)
      });
      return;
    }
    if (newest_entry.sub_entries.size()  == newest_entry.all_semantical_analysis_rules.size()) 
    {
    newest_entry.all_semantical_analysis_rules.back().push_back(  std::move(semantical_rule_entry));
    }
    else{
      std::cerr<<"count mismatch between the amount of entries in \"all_semantical_analysis_rules\"  and \"sub_entries\" data members of the newest entry"<<std::endl;
      throw std::runtime_error("add_semantic_rule_for_newest_sub_entry error");
    }
  }
  absolute_base::Non_terminal_name_entry& All_non_terminal_entries_implementation::get_current_non_term_entry(int index) {
      //precondition:index is not negative and list_of_all_non_term_entries_for_fast_traversal is in a valid state
      //postcondition: returns a non term entry at the given index to the caller
      return list_of_all_non_term_entries_for_fast_traversal[index];

  }
  absolute_base::Non_terminal_name_entry& All_non_terminal_entries_implementation::get_current_nested_non_term_entry(int index_for_nested_non_term, int index) {
      //precondition:the given indexes are not negative and list_of_all_non_term_entries_for_fast_traversal is in a valid state
    //postcondition: returns a nested non term entry at the index "[index][index_for_nested_non_term]"
      return *(list_of_all_non_term_entries_for_fast_traversal[index].sub_entries[index_for_nested_non_term]);

  }
