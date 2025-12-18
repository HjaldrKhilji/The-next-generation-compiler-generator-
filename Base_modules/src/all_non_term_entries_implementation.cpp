module;
#include <iostream>


#include <map>


#include <sstream>

#include <string>

#include <vector>
#include<regex>
module all_declarations;

using absolute_base::Settings_for_semantical_rules;
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


void absolute_base::All_non_terminal_entries::print_all_content() {
    //precondition:*this's  invariant is valid
    
    std::cout << "number of entries: " << list_of_all_non_term_entries_for_fast_traversal.size() << std::endl;
    for (auto current_entry : list_of_all_non_term_entries_for_fast_traversal) {
        std::cout << "current entry:" << std::endl;
        std::cout << current_entry.name << " ";
        std::cout << current_entry.pattern << " ";
        int index = 0;
        std::cout << "syntaxical data:" << std::endl;
        for (auto wrapped_sub_entries = current_entry.sub_entries.begin(); wrapped_sub_entries + index != current_entry.sub_entries.end(); ++index) {
            std::cout << "number of nested entries in current entry: " << current_entry.sub_entries.size() << std::endl;

            auto unwrapped_sub_entries = wrapped_sub_entries[index].get();
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

void absolute_base::All_non_terminal_entries::add_non_term_symbol_name(std::string name) {
  //precondition: "name" paremeter can not be empty
  //precondition: list_of_all_non_term_entries_for_fast_traversal is in a valid state 
  //postcondition: an entry is added to the "traversal list" only, hence you can only access the entry by traversing the deque,
  //the entry entered dose'nt contain anything other than the name.

    list_of_all_non_term_entries_for_fast_traversal.push_back(Non_terminal_name_entry {
      name,
      {},
      {},
      {}
    });

  }
  void absolute_base::All_non_terminal_entries::add_non_term_pattern_for_newest_entry(std::string pattern) {
    //precondition: "pattern" paremeter dose'nt contain a invalid regex expression
    //precondition: list_of_all_non_term_entries_for_fast_traversal is in a valid state and contains atleast a single entry
    //precondition: map_for_fast_retrival_of_entries is also in a valid state, and is amenable(mutable) enough to have a data inserted into it
    //postcondition: list_of_all_non_term_entries_for_fast_traversal gets the pattern for its latest entry entered
    //postcondition: map_for_fast_retrival_of_entries gets the "name" data member of the latest entry as key and the latest entry itself
    //entered as a entry into map_for_fast_retrival_of_entries
    auto & newest_entry = list_of_all_non_term_entries_for_fast_traversal.back();
    newest_entry.pattern = pattern;
    //notice that this also a adds the entry to the map for fast retrival:
    map_for_fast_retrival_of_entries.insert({
      newest_entry.name,
      newest_entry
    });

  }
  std::string & absolute_base::All_non_terminal_entries::get_pattern_of_nested_non_term_symbol_pattern(std::string sub_symbol_name) {
  //precondition: "sub_symbol_name" paremeter can not be empty
  //precondition: map_for_fast_retrival_of_entries is in a valid state and is not empty
  //postcondition: the entry corrsponding to the key "sub_symbol_name" from map_for_fast_retrival_of_entries, and the copy of the 
  // "pattern" data member of that entry is returned

    if (!map_for_fast_retrival_of_entries.contains(sub_symbol_name)) {
      std::cerr<<"no pattern founds"<<std::endl;
      throw std::runtime_error("get_pattern_of_nested_non_term_symbol_pattern error");
    }

    return map_for_fast_retrival_of_entries.at(sub_symbol_name).get().pattern;

  }
  std::reference_wrapper < std::string > absolute_base::All_non_terminal_entries::get_parmenant_name_of_nested_non_term_symbol_pattern(std::string sub_symbol_name) {
//precondition: "sub_symbol_name" paremeter can not be empty
  //precondition: map_for_fast_retrival_of_entries is in a valid state and is not empty
  //postcondition: the entry corrsponding to the key "sub_symbol_name" from map_for_fast_retrival_of_entries, and the reference wrapper containing
  //a reference to the data member "pattern" of the entry is returned
    if (!map_for_fast_retrival_of_entries.contains(sub_symbol_name)) {
      std::cerr<<"no pattern found"<<std::endl;
      throw std::runtime_error("get_parmenant_name_of_nested_non_term_symbol_pattern error");

    }

    return map_for_fast_retrival_of_entries.at(sub_symbol_name).get().name;

  }
  void absolute_base::All_non_terminal_entries::add_nested_non_term_symbol_to_the_newest_entry(std::string sub_symbol_name) {
    //precondition: "sub_symbol_name" paremeter can not be empty
    //precondition: map_for_fast_retrival_of_entries is in a valid state and is not empty
    //precondition: list_of_all_non_term_entries_for_fast_traversal is in a valid state and contains atleast a single entry
    //postcondition: the "sub_entries" field has a reference wrapper that points to a nested field appended to itself


    auto & newest_entry = list_of_all_non_term_entries_for_fast_traversal.back();
    auto & sub_entry_that_corrosponds_to_sub_name = map_for_fast_retrival_of_entries.at(sub_symbol_name);

    newest_entry.sub_entries.push_back(sub_entry_that_corrosponds_to_sub_name.get());

  }

  void absolute_base::All_non_terminal_entries::add_semantic_rule_for_newest_sub_entry(const absolute_base::Semantical_analyzer_config_entry && semantical_rule_entry) {
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
  absolute_base::Non_terminal_name_entry absolute_base::All_non_terminal_entries::get_current_non_term_entry(int index) {
      //precondition:index is not negative and list_of_all_non_term_entries_for_fast_traversal is in a valid state
      //postcondition: returns a non term entry at the given index to the caller
      return list_of_all_non_term_entries_for_fast_traversal[index];

  }
  absolute_base::Non_terminal_name_entry absolute_base::All_non_terminal_entries::get_current_nested_non_term_entry(int index_for_nested_non_term, int index) {
      //precondition:the given indexes are not negative and list_of_all_non_term_entries_for_fast_traversal is in a valid state
    //postcondition: returns a nested non term entry at the index "[index][index_for_nested_non_term]"
      return list_of_all_non_term_entries_for_fast_traversal[index].sub_entries[index_for_nested_non_term].get();

  }
