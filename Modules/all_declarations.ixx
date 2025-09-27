module;
#include <iostream>

#include<functional>

#include <map>

#include <regex>

#include <sstream>

#include <string>

#include <vector>
#include <deque>

export module all_declarations;//for c++ noobs, including myself, the module name dosent have to be the same as file name.

//The reasons that I have so many incomplete todos is because my program's design is still evolving, tho the principle remains, hence I cant make my code readable before I finsih my design.
//gcc dosent support import std; , so I didnt use it
//why is the design not complete? its not because I dont know how to make this, but because some limitations of myself unfold, so while I have a idea of how to make it, there will be some limtiations stopping me from knowing exactly how to.
//semantic analyzer has the worst error handling when compared to all the other parts of my code, so that a TODO
//todo: add more error handling for all stream operations(in the implementation) and make exceptions give some sort of gurrenties of crashes, or on recovery, instead of just undefined
//todo: MAKE THE implementation CODE READABLE 

//My reasons for doing stuff in a certian way:

//my comments in my source file might be hard to understand, but that because They are meant to be unambiguous and the subjective judgement of what unambiguous was done
//on my test, so sorry if my taste does not matches your, and also my grammar and english kind of sucks ass, to be honest.
//I only use regex for lexcial anaylsis, and semantical checks
//PLEASE NOTE THAT I DIDNT USE FLAT MAPS FOR NON TERM ENTRIES, OR REGEX FUNCTIONS FOR ESCAPING STRINGS because:
/*
1.Flat maps arent insert ordered
2.REGEX Functions for escaping strings are slow in C++, and I dont trust the boost library alternative functions
3.REGEX functions didnt fullfill all my needs in a generic way, and I needed something more generic, like a regex 2.0
I am still using REGEX underneath the regex 2.0, at the time that I added this comment, regex underneath regex 2.0 isnt fully done
its regex 2.0 because the escape_string is highly generic, and the way I use it is also generic, The true regex 2.0 is the semantical analyser.
*/
/* 
I used refernece wrappers because I Just dont like using pointers, in code that is meant to be low level and hard to read
*/
//I use istringstream a lot because of flexibility in error handling, parsing, but also for the sake of extending my code with ease.
//I also believe that istringstream are actually faster for my case, again, I cant confirm, but the reason that I think this way is:
/*

it can help me read input directly into any type, I do get that some conversion will happen to the target type, but that is done using the standard
library, so its still not that bad, in terms of performance because standard library tends to have optimized implementations

think of how the stack operations in assembly can be used to make the logic of your code concise, and your code faster in doing so

in this case, I want to make sure that when I read input, I move it as well, I also want to make sure that I can keep the logic concise.



my future self: please dont replace istringstream, until the code is finished to the point that someone will actually use it, and question it for performance, so yeah, I need it!
 
to my future self:
if you prove that istringstreams are inefficient, and escape_string function is too then just dont change the design, and try:
1.defining your own line_stream whose interface is the same as istringstreams, make sure the istringstream alternative is well defined. 
2.dont remove escape_string itself, just change the implementation

with the current design, I can optimize the code, if my code is ever proven to be slower than what it can be.
so dont change the design!!!!!!!!

optimize only after the compiler generator is done

again these are just my opinion, dont judge

*/
// so yeah thats all the reasons that I have
export{
 namespace common_functions {
    using escape_charactor_function_wrapper_type = std:: function < void(
    std::string & input_string, size_t & where_is_it_found) > ;
  void escape_string(std::string & input_string,
    const std::vector < std::string > & strings_to_be_replaced,
      const std::vector < escape_charactor_function_wrapper_type >
        function_to_be_run_for_each) ;
       std::string read_identifier(std::istringstream & line_stream);
      
}
 enum class settings_for_semantical_rules : short {
     NONE = 0,
     check_exist = 1,
     check_dont_exist = 2,
     check_atleast = 4,
     check_exact = 8
 };
 settings_for_semantical_rules operator|(settings_for_semantical_rules a, settings_for_semantical_rules b) {
     using underlying_t = std::underlying_type_t<settings_for_semantical_rules>;
     return static_cast<settings_for_semantical_rules>(
         static_cast<underlying_t>(a) | static_cast<underlying_t>(b)
         );
 }

 settings_for_semantical_rules& operator|=(settings_for_semantical_rules& a, settings_for_semantical_rules b) {
     a = a | b;
     return a;
 }
 settings_for_semantical_rules operator^(settings_for_semantical_rules a, settings_for_semantical_rules b) {
     using underlying_t = std::underlying_type_t<settings_for_semantical_rules>;
     return static_cast<settings_for_semantical_rules>(
         static_cast<underlying_t>(a) ^ static_cast<underlying_t>(b)
         );
 }

 settings_for_semantical_rules& operator^=(settings_for_semantical_rules& a, settings_for_semantical_rules b) {
     a = a ^ b;
     return a;
 }
  struct Semantical_analyzer_config_entry {

  std::reference_wrapper < std::string > name_of_non_term_symbol_to_check;
  std::string the_pattern_to_check;
  settings_for_semantical_rules all_settings;

  unsigned int minimum_amount_of_matches;
  unsigned int maximum_amount_of_matches;
};
  struct Non_terminal_name_entry {

  std::string name;
  std::string pattern;
  std::vector < std::reference_wrapper < Non_terminal_name_entry >> sub_entries;
  std::vector < std::vector < Semantical_analyzer_config_entry >> all_semantical_analysis_rules;

};
   
  class All_non_terminal_entries {
public: void add_non_term_symbol_name(std::string name) ;//this adds the non term symbol only to fast traversal list
void add_non_term_pattern_for_newest_entry(std::string pattern);//this adds the newest element to the fast search map as well, after it has enter the pattern into the newest element traversal list
std::string & get_pattern_of_nested_non_term_symbol_pattern(std::string sub_symbol_name);//this gets the pattern for a name, from the map, and the reason I say for nested non term symbol is because it is generally used for finding the symbols for nested non term symbols
void add_nested_non_term_symbol_to_the_newest_entry(std::string sub_symbol_name);//this just takes a string, which it assumes to be the name of a non term symbol, and finds the non term symbol(from the map), and adds it's reference into the nested non term symbol list of the newest entry
void add_semantic_rule_for_newest_sub_entry(const Semantical_analyzer_config_entry && semantical_rule_entry);//this is my favorite(I spend 4 days on making this), so all it does is that it finds the latest entry from the fast traversal list, then finds the latest sub entry in that, and adds this semantic rule entry for that sub entry 
std::reference_wrapper < std::string > get_parmenant_name_of_nested_non_term_symbol_pattern(std::string sub_symbol_name);//returns the permanent reference to a non term name
void print_all_content();
private:
//the three types and aliases are just implementation details, please ignore it tho, if you arent implementing this class.
using reference_to_string = std::reference_wrapper < std::string > ;
  using reference_to_Non_terminal_name_entry = std::reference_wrapper < Non_terminal_name_entry > ;
    struct Function_object_class_to_compare_underlying_objects_of_a_reference {
    bool operator()(const std::reference_wrapper < std::string > & a,
      const std::reference_wrapper < std::string > & b) const {
      return a.get() < b.get();
    }
  };
 
  //it should be obvious that the map is for fast lookups, and deque is for fast traversal:
 std::map < reference_to_string,
  reference_to_Non_terminal_name_entry,
  Function_object_class_to_compare_underlying_objects_of_a_reference > map_for_fast_retrival_of_entries;
  std::deque < Non_terminal_name_entry > list_of_all_non_term_entries_for_fast_traversal;

};
   class Base_class_for_all_parts_of_compiler_gen{//this class wont be implemented, but instead inherited by another module that would finally implement it after inheriting the signature.
    //that module will also export this module
    public:
   ~Base_class_for_all_parts_of_compiler_gen() {
    if (is_input_stream_owned) delete input_stream;

    if (is_output_stream_owned) delete output_stream;
  }//if the streams are owned, then deallocate them
  Base_class_for_all_parts_of_compiler_gen(std::istream & a, std::ostream & b): current_line_number {},
  all_entries {},
  line_stream {
    ""
  },
  input_stream {
    &a
  },
  output_stream {
    &b
  },
  is_input_stream_owned {
    false
  },
  is_output_stream_owned {
    false
  } {}
  Base_class_for_all_parts_of_compiler_gen(std::istream * a, std::ostream * b): current_line_number {},
  all_entries {},
  line_stream {
    ""
  },
  input_stream {
    a
  },
  output_stream {
    b
  },
  is_input_stream_owned {
    true
  },
  is_output_stream_owned {
    true
  } {}
  Base_class_for_all_parts_of_compiler_gen(std::istream  *a, std::ostream &  b): current_line_number {},
  all_entries {},
  line_stream {
    ""
  },
  input_stream {
    a
  },
  output_stream {
    &b
  },
  is_input_stream_owned {
    true
  },
  is_output_stream_owned {
    false
  } {}
  Base_class_for_all_parts_of_compiler_gen(std::istream & a, std::ostream * b): current_line_number {},
  all_entries {},
  line_stream {
    ""
  },
  input_stream {
    &a
  },
  output_stream {
    b
  },
  is_input_stream_owned {
    false
  },
  is_output_stream_owned {
    true
  } {}
  void change_input_stream(std::istream & new_input_stream);
  void change_output_stream(std::ostream & new_output_stream);
  void change_input_stream(std::istream * new_input_stream);
  void change_output_stream(std::ostream * new_output_stream);
  void print_all_parsed_input_for_testing();
    void get_and_parse_input();//fetches and parses all lines using current line, and handle all the exceptions thrown while trying to do so, my advice to the implementor
    //is to define separate functions for each errors, and pass the state needed to fix that error to that function.

  protected:
    void change_current_line();//changes line by calling get_raw_input and parse_raw_input
  void parse_raw_input();//escapes all the escape charactos, including the ones for declaring syntax rules, and the implementation for that in turn parse semantic rules, and that does the layers below.
    std::istringstream & current_line();//fetches a new raw line using get_raw_input, parses it using get_and_parse_input

  void semantical_analyzer_entry_reader();//simply reads semantic rules using the three functions below, and the format of the input is:
  //name of the nested non term symbol entry, whose pattern match to look into, the pattern to look for in that match, and last but not least how somoe wild regex charactors like +, *, {1,2}, !(tho ! is not a regex wild charactor, I use it to add a option, that the pattern, and the wild charactor combination should not be matches, to that, just add ! in the start of the configuration input) 
  void parse_config_for_semantic_entry(settings_for_semantical_rules & settings_for_current_config, std::istringstream && line_stream, unsigned int & minimum_amount_of_Matches, unsigned int & maximum_amount_of_matches);
  std::string take_space_terminated_input_and_escape_it();
  std::string return_raw_config_for_pattern();//all the functions meant for escaping, are all for inputs of regex pattern's only:
  void escape_backslash_capital_u_by_reading_nested_symbols(std::string & input_string, size_t & where_is_it_found);  // a conviniance escape charactor for simply reading a non term symbol's name, and using that to get its pattern, and adding the pattern to the current pattern

  void escape_backslash_capital_a_by_reading_nested_symbols(std::string & input_string, size_t & where_is_it_found);  // a conviniance escape charactor for simply reading a non term symbol's name, and using that to get its pattern, and adding the pattern to the current pattern, and adding the nested non term symbol entry to the list of entries of the current entry, this escape charactor handler should onl ybe used for patterns of non term name entries, not of semantic checking. after doing that, it then takes semantic entries
  
 
  void escape_backslash_capital_n(std::string & input_string, size_t & where_is_it_found);// a conviniance function so that you can add the input form next line, into the place where you wrote \N
   //the code below is simply if you want the line below to be pasted in place, for example:
  /*
  a \N a
  sta mur ghayama 
  becomes:
  a  sta mur ghayama a
  */
  std::string get_raw_input();//simply gets raw input, and returns it, as if it is MAAAAAAAAAAAAAAAGICCCCCCCC 
   using reference_to_string = std::reference_wrapper < std::string > ;
  using reference_to_Non_terminal_name_entry = std::reference_wrapper < Non_terminal_name_entry > ;

  

    int current_line_number;
  All_non_terminal_entries all_entries;
  std::istringstream line_stream;
  std::istream * input_stream;
  std::ostream * output_stream;
  bool is_input_stream_owned;
  bool is_output_stream_owned;
  //this design will expand with time, and I will introduce cmake config files when it does
};}
