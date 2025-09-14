#include <iostream>

#include<functional>

#include <map>

#include <regex>

#include <sstream>

#include <string>

#include <vector>
 //todo: MAKE THE CODE READABLE 
//I only use regex for lexcial anaylsis, and semantical checks
 //PLEASE NOTE THAT I DIDNT USE FLAT MAPS FOR NON TERM ENTRIES, OR REGEX FUNCTIONS FOR ESCAPING STRINGS because:
 /*
 1.Flat maps arent insert ordered
 2.REGEX Functions for escaping strings are slow in C++, and I dont trust the boost library alternative functions
 3.REGEX functions didnt fullfill all my needs in a generic way, and I needed something more generic, like a regex 2.0
 I am still using REGEX underneath the regex 2.0, at the time that I added this comment, regex underneath regex 2.0 isnt fully done
 its regex 2.0 because the escape_string is highly generic, and the way I use it is also generic, The true regex 2.0 is the semantical analyser,
 which again at the time of this commit isnt full completed
 */
/* 
I used refernece wrappers because I Just dont like using pointers, in code that is meant to be low level and hard to read
*/
namespace common_functions {
  using escape_charactor_function_wrapper_type= std::function<void(
          std::string & input_string, size_t & where_is_it_found)>;
  void escape_string(std::string & input_string,
    const std::vector < std::string > & strings_to_be_replaced,
      const std::vector < escape_charactor_function_wrapper_type >
        function_to_be_run_for_each) {
    for (size_t index = 0; index < strings_to_be_replaced.size(); index++) {
      size_t position_of_the_match_found_last = 0;
      while ((position_of_the_match_found_last = input_string.find(
          strings_to_be_replaced[index],
          position_of_the_match_found_last)) != std::string::npos) {
        function_to_be_run_for_each[index](input_string,
          position_of_the_match_found_last);
      }
    }
  }
  std::string read_identifier(std::istringstream & line_stream) {
    std::string identifier;
    char c;
    while (line_stream.get(c) && (isspace(c)));
    line_stream.putback(c);
    for (; line_stream.get(c) && (isalnum(c) || c == '_'); identifier += c);
    line_stream.putback(c);
    if (identifier.empty()) {
      throw std::runtime_error("empty non terminal symbol name");
    }
    return identifier;

  }
};
struct Semantical_analyzer_config_entry{
  std::reference_wrapper < std::string > name_of_non_term_symbol_to_check;
  std::reference_wrapper < std::string > the_pattern_to_check;

  bool whether_to_checK_exist_or_not_exist;//1 for if you want to check if it exist, and 0 if you dont
  bool whether_to_check_atleast_or_exact;//1 if you want to check atleast, and 0 if you dont
  
  
  unsigned int number_of_times_checks;
};
struct Non_terminal_name_entry {

  std::string name;
  std::string pattern;
  std::vector < std::reference_wrapper < Non_terminal_name_entry >> sub_entries;
  std::vector < std::vector < Semantical_analyzer_config_entry >> Semantical_analysis_rules;

};

class All_non_terminal_entries {
  public: void add_non_term_symbol_name(std::string name) {
    list_of_all_non_term_entries_for_fast_traversal.push_back(Non_terminal_name_entry {
      name,
      {},
      {},
      {}
    });

  }
  void add_non_term_pattern_for_newest_entry(std::string pattern) {
    auto & newest_entry = list_of_all_non_term_entries_for_fast_traversal.back();
    newest_entry.pattern = pattern;
    //notice that this also a adds the entry to the map for fast retrival:
    map_for_fast_retrival_of_entries.insert({
      newest_entry.name,
      newest_entry
    });

  }
  std::string & get_pattern_of_nested_non_term_symbol_pattern(std::string sub_symbol_name) {
    if (!map_for_fast_retrival_of_entries.contains(sub_symbol_name)) {
      throw std::runtime_error("no pattern founds, output is undefined!");
    }

    return map_for_fast_retrival_of_entries.at(sub_symbol_name).get().pattern;

  }
  void add_nested_non_term_symbol_to_the_newest_entry(std::string sub_symbol_name) {
    //call only after calling get_pattern_of_nested_non_term_symbol_pattern to make sure the string does exist
    auto & newest_entry = list_of_all_non_term_entries_for_fast_traversal.back();
    auto & sub_entry_that_corrosponds_to_sub_name = map_for_fast_retrival_of_entries.at(sub_symbol_name);

    newest_entry.sub_entries.push_back(sub_entry_that_corrosponds_to_sub_name.get());

  }

  void add_semantic_rule_for_newest_sub_entry(Semantical_analyzer_config_entry semantical_rule_entry) {
    auto & newest_entry = list_of_all_non_term_entries_for_fast_traversal.back();;
    newest_entry.Semantical_analysis_rules.back().push_back(semantical_rule_entry);

  }
 
  void print_all_content() {
    for (auto current_entry: list_of_all_non_term_entries_for_fast_traversal) {
      std::cout << current_entry.name << " ";
      std::cout << current_entry.pattern << " ";

      for (auto wrapped_sub_entries_in_the_current_entry: current_entry.sub_entries) {
        auto unwrapped_sub_entries_in_the_current_entry = wrapped_sub_entries_in_the_current_entry.get();
        std::cout << unwrapped_sub_entries_in_the_current_entry.name << unwrapped_sub_entries_in_the_current_entry.pattern << " ";

      }

    }
    std::cout << std::endl;
  }
  private: 
  using reference_to_string = std::reference_wrapper <  std::string > ;
  using reference_to_Non_terminal_name_entry = std::reference_wrapper <  Non_terminal_name_entry > ;
  struct Function_object_class_to_compare_underlying_objects_of_a_reference {
    bool operator()(const std::reference_wrapper < std::string > & a,
      const std::reference_wrapper < std::string > & b) const {
      return a.get() < b.get();
    }
  };
  std::map < reference_to_string,
  reference_to_Non_terminal_name_entry,
  Function_object_class_to_compare_underlying_objects_of_a_reference > map_for_fast_retrival_of_entries;
  std::deque < Non_terminal_name_entry > list_of_all_non_term_entries_for_fast_traversal;
};

class Compiler {
  public: std::string get_raw_input() {
    std::string raw_input {};
    std::getline( * input_stream, raw_input);

    current_line_number++;
    return std::move(raw_input);
  }

  class Function_object_to_escape_escape_charactors {
    public:
      //lambda has to be used with common_functions::escape_string() function

      std::string string_to_be_replaced_with;
    size_t size_of_the_replacement_of_escape_string;

    size_t size_of_escape_charactor;
    Function_object_to_escape_escape_charactors(std::string && y, size_t x): string_to_be_replaced_with {
      y
    },
    size_of_the_replacement_of_escape_string {
      y.length()
    },
    size_of_escape_charactor {
      x
    } {}

    void operator()(std::string & input_string, size_t & where_is_it_found) {
      input_string.replace(where_is_it_found,
        size_of_escape_charactor, string_to_be_replaced_with);
      where_is_it_found += size_of_the_replacement_of_escape_string;
    }
  };
  //the code below is simply if you want the line below to be pasted in place, for example:
  /*
  a \N a
  sta mur ghayama 
  becomes:
  a  sta mur ghayama a
  */

    void escape_backslash_capital_N (std::string & input_string, size_t & where_is_it_found)  {
    constexpr size_t size_of_common_escape_charactors = 2;

    std::string temp_input =  get_raw_input();

    input_string.replace(
      where_is_it_found,
      size_of_common_escape_charactors,
      temp_input);
    
  };

  //note that the layer below lexical analysis is syntaxical analysis, and the config generation for that
  // is handled by the escape characotor \A, the code below is the function to handle \A, this function then calls semantical 
  //config analyzer parser


std::string return_raw_info_where_to_look_in_and_config_on_how(){
  constexpr int size_of_common_escape_charactors=1;
 std::string name = common_functions::read_identifier(line_stream);
      std::string where_to_look_in_config;
      line_stream>>where_to_look_in_config;
      common_functions::escape_string(
      where_to_look_in_config, {
        "+",
        "*",
        "?"
        
      },{
       Function_object_to_escape_escape_charactors {
          "{1,}",
          size_of_common_escape_charactors
        },
     Function_object_to_escape_escape_charactors {
          "{0,}",
          size_of_common_escape_charactors
        },
       Function_object_to_escape_escape_charactors {
          "{0,0}",
          size_of_common_escape_charactors
        }
      }
    );
    
    return std::move(name);
  }
void semantical_analyzer_rules_reader(){
    constexpr size_t size_of_common_escape_charactors = 2;

    std::string semantic_pattern_to_check;
    line_stream >> semantic_pattern_to_check;
    common_functions::escape_string(
      semantic_pattern_to_check, {
        "\\\\",
        "\\S",
        "\\t",
        "\\n",
        "\\N"
      
      },
      {
      Function_object_to_escape_escape_charactors {
          "\\",
          size_of_common_escape_charactors
        },
        Function_object_to_escape_escape_charactors {
          " ",
          size_of_common_escape_charactors
        },

        Function_object_to_escape_escape_charactors {
          "\t",
          size_of_common_escape_charactors
        },
        Function_object_to_escape_escape_charactors {
          "\n",
          size_of_common_escape_charactors
        },
        Function_object_to_escape_escape_charactors {
          "\t",
          size_of_common_escape_charactors
        },
        [this](std::string& input_string, size_t &where_found){this->escape_backslash_capital_N(input_string, where_found);}

      });
      std::string raw_info_on_where_to_look_in_and_how=return_raw_info_where_to_look_in_and_config_on_how();
      std::istringstream line_stream {raw_info_on_where_to_look_in_and_how};
     // Semantical_analyzer_config_entry entry_to_enter; notice the default constructor of the class type is deleted, this issue will be solved, though, by me
      char parse_config_one_by_one;
      if(line_stream>>parse_config_one_by_one){
      switch(parse_config_one_by_one){
        case '?':

        break;
        case '{':

        break;
        default:
       
        break;
      }

      }

     
}

  void escape_backslash_capital_A_by_reading_nested_symbols (std::string & input_string, size_t & where_is_it_found)  {
    constexpr size_t size_of_common_escape_charactors = 2;

    std::string name = common_functions::read_identifier(line_stream);
    std::string the_nested_non_term_entry_pattern = all_entries.get_pattern_of_nested_non_term_symbol_pattern(name);
    std::string pattern_corrresponding_to_nested_name = "(" + the_nested_non_term_entry_pattern + ")";
    input_string.replace(
      where_is_it_found,
      size_of_common_escape_charactors, pattern_corrresponding_to_nested_name);
    all_entries.add_nested_non_term_symbol_to_the_newest_entry(name);

    where_is_it_found += pattern_corrresponding_to_nested_name.length();

  }
  void parse_raw_input() {
    constexpr size_t size_of_common_escape_charactors = 2;
    std::string non_terminal_name = common_functions::read_identifier(line_stream);
    std::string non_terminal_pattern;
    line_stream >> non_terminal_pattern;
    all_entries.add_non_term_symbol_name(non_terminal_name);
    //replace \\ with \, and \\N with what ever is in the next line
    //replace \\n with \n(newline),\\S with space, \\t with a tab
    //\\A with the pattern corrosponding to the non terminal name after
    //\\A, and push that name into   current_non_terminal_name_entry
    //notice where_is_it_found variable is updated in each lambda for the next loop
    //to read content after the text replaced.
    common_functions::escape_string(
      non_terminal_pattern, {
        "\\\\",
        "\\N",
        "\\n",
        "\\S",
        "\\t",
        "\\A",
      }, {
        Function_object_to_escape_escape_charactors {
          "\\",
          size_of_common_escape_charactors
        },
        [this](std::string& input_string, size_t &where_found){this->escape_backslash_capital_N(input_string, where_found);}

        ,
        Function_object_to_escape_escape_charactors {
          "\n",
          size_of_common_escape_charactors
        },
        Function_object_to_escape_escape_charactors {
          " ",
          size_of_common_escape_charactors
        },
        Function_object_to_escape_escape_charactors {
          "\t",
          size_of_common_escape_charactors
        },
        [this](std::string& input_string, size_t &where_found){this->escape_backslash_capital_A_by_reading_nested_symbols(input_string, where_found);}

        
      }

    );
    all_entries.add_non_term_pattern_for_newest_entry(non_terminal_pattern);
  }

  void print_all_parsed_input_for_testing() {
    all_entries.print_all_content();
  }
  void change_output_stream(std::ostream * new_output_stream) {
    output_stream = new_output_stream;
    is_output_stream_owned = true;

  }
  void change_input_stream(std::istream * new_input_stream) {
    input_stream = new_input_stream;
    is_input_stream_owned = true;

  }
  void change_output_stream(std::ostream & new_output_stream) {
    output_stream = & new_output_stream;
    is_input_stream_owned = false;
  }

  void change_input_stream(std::istream & new_input_stream) {
    input_stream = & new_input_stream;
    is_input_stream_owned = false;
  }

  void change_current_line() {
    line_stream.clear();

    line_stream.str(get_raw_input());
    parse_raw_input();

  }
  std::istringstream & current_line() {
    return line_stream;
  }
  void get_and_parse_input() {
    do {
      try {
        change_current_line();

      } catch (const std::ios_base::failure & e) {
        std::cerr << "File stream error: " << e.what() << "\n" << current_line_number << "\n\n\n";
        return;
      } catch (const std::runtime_error & e) {
        std::cerr << "Runtime error: " << e.what() << current_line_number << "\n\n\n";

        return;
      }
      //notice an extra flush using std::endl isnt required while using cerr stream in the code above because cerr is flush automatically
    } while (line_stream.good() == true);

  } // parse the whole file

  Compiler(std::istream & a, std::ostream & b): current_line_number {},
  all_entries {},
  line_stream {
    ""
  },
  input_stream {
    &
    a
  },
  output_stream {
    &
    b
  },
  is_input_stream_owned {
    false
  },
  is_output_stream_owned {
    false
  } {}
  Compiler(std::istream * a, std::ostream * b): current_line_number {},
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
  ~Compiler() {
    if (is_input_stream_owned) delete input_stream;

    if (is_output_stream_owned) delete output_stream;
  }

  private:

    int current_line_number;
  All_non_terminal_entries all_entries;
  std::istringstream line_stream;
  std::istream * input_stream;
  std::ostream * output_stream;
  bool is_input_stream_owned;
  bool is_output_stream_owned;
};

int main() {
  //test:
  Compiler lexer {
    std::cin, std::cout
  };
  lexer.get_and_parse_input();
  lexer.get_and_parse_input();
  lexer.print_all_parsed_input_for_testing();
  return 0;
}

