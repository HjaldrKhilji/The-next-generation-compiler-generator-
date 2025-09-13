#include <iostream>

#include<functional>

#include <map>

#include <regex>

#include <sstream>

#include <string>

#include <vector>
 //todo: MAKE THE CODE READABLE 
namespace common_functions {
  void escape_string(std::string & input_string,
    const std::vector < std::string > & strings_to_be_replaced,
      const std::vector < std:: function < void(
          std::string & input_string, size_t & where_is_it_found) >>
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
    for (; line_stream.get(c) && (isalnum(c) || c == '_'); identifier += c);
    line_stream.putback(c);
    if (identifier.length() == 0) {
      throw std::runtime_error("empty non terminal symbol name");
    }
    return identifier;

  }
};
struct Non_terminal_name_entry {

  std::string name;
  std::string pattern;
  std::vector < std::reference_wrapper < Non_terminal_name_entry >> sub_entries;
  std::vector < std::vector < std::string >> the_semantics_of_sub_names;

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
    if (map_for_fast_retrival_of_entries.contains(sub_symbol_name)) {
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

  void add_semantic_rule_for_newest_sub_entry(std::string sub_symbol_name) {
    auto & newest_entry = list_of_all_non_term_entries_for_fast_traversal.back();;
    newest_entry.the_semantics_of_sub_names.back().push_back(sub_symbol_name);

  }
  Non_terminal_name_entry & return_current_entry() {
    if (list_of_all_non_term_entries_for_fast_traversal.empty()) {
      throw std::runtime_error("no entries inputed, the OUTPUT is undefined");

    }
    return list_of_all_non_term_entries_for_fast_traversal[entry_number];

  }
  void change_current_entry() {
    entry_number++;
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
  private: int entry_number {};
  using reference_to_string = std::reference_wrapper < std::string > ;
  using reference_to_Non_terminal_name_entry = std::reference_wrapper < Non_terminal_name_entry > ;
  struct Function_object_class_to_compare_underlying_objects_of_a_reference {
    bool operator()(const std::reference_wrapper < std::string > & a,
      const std::reference_wrapper < std::string > & b) const {
      return a.get() < b.get();
    }
  };
  std::map < reference_to_string,
  reference_to_Non_terminal_name_entry,
  Function_object_class_to_compare_underlying_objects_of_a_reference > map_for_fast_retrival_of_entries;
  std::vector < Non_terminal_name_entry > list_of_all_non_term_entries_for_fast_traversal;
};

class Compiler {
  public: std::string get_raw_input() {
    std::string raw_input {};
    std::getline( * input_stream, raw_input);

    current_line_number++;
    return raw_input;
  }
  //to be done later:
  // void Parse_semantical_rules(){
  //     std::string semantic_input_for_current_nested_non_term_symbol;
  //     line_stream>>semantic_input_for_current_nested_non_term_symbol;
  //     common_functions::escape_string(
  //       non_terminal_pattern, {
  //         "\\\\",
  //         "\\N",
  //         "\\n",
  //         "\\S",
  //         "\\t",
  //         "\\A",
  //       }, {

  //         [](std::string & input_string, size_t & where_is_it_found) -> void {
  //           input_string.replace(where_is_it_found,
  //             size_of_common_escape_charactors, "\\");
  //           where_is_it_found += size_of_the_replacement_of_escape_char;
  //         },
  //         [this](std::string & input_string,
  //           size_t & where_is_it_found) -> void {
  //           std::string temp_input = this -> get_raw_input();

  //           input_string.replace(
  //             where_is_it_found,
  //             size_of_common_escape_charactors,
  //              temp_input);
  //           where_is_it_found += temp_input.length();
  //         },
  //         [](std::string & input_string, size_t & where_is_it_found) -> void {
  //           input_string.replace(
  //             where_is_it_found,
  //             size_of_common_escape_charactors, "\n");
  //           where_is_it_found += size_of_the_replacement_of_escape_char;
  //         },
  //         [](std::string & input_string, size_t & where_is_it_found) -> void {
  //           input_string.replace(
  //             where_is_it_found,
  //             size_of_common_escape_charactors, " ");
  //           where_is_it_found += size_of_the_replacement_of_escape_char;
  //         },
  //         [](std::string & input_string, size_t & where_is_it_found) -> void {
  //           input_string.replace(
  //             where_is_it_found,
  //             size_of_common_escape_charactors, "\t");
  //           where_is_it_found += size_of_the_replacement_of_escape_char;
  //         },
  //         [ &non_terminal_name,& non_terminal_pattern, this](std::string & input_string,
  //           size_t & where_is_it_found) -> void {

  //         }

  //       });

  //   }

  void parse_raw_input() {
    constexpr size_t size_of_common_escape_charactors = 2;
    constexpr size_t size_of_the_replacement_of_escape_char = 1;
    std::string non_terminal_name = common_functions::read_identifier(this -> line_stream);
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

        [](std::string & input_string, size_t & where_is_it_found) -> void {
          input_string.replace(where_is_it_found,
            size_of_common_escape_charactors, "\\");
          where_is_it_found += size_of_the_replacement_of_escape_char;
        },
        [this](std::string & input_string,
          size_t & where_is_it_found) -> void {
          std::string temp_input = this -> get_raw_input();

          input_string.replace(
            where_is_it_found,
            size_of_common_escape_charactors,
            temp_input);
          where_is_it_found += temp_input.length();
        },
        [](std::string & input_string, size_t & where_is_it_found) -> void {
          input_string.replace(
            where_is_it_found,
            size_of_common_escape_charactors, "\n");
          where_is_it_found += size_of_the_replacement_of_escape_char;
        },
        [](std::string & input_string, size_t & where_is_it_found) -> void {
          input_string.replace(
            where_is_it_found,
            size_of_common_escape_charactors, " ");
          where_is_it_found += size_of_the_replacement_of_escape_char;
        },
        [](std::string & input_string, size_t & where_is_it_found) -> void {
          input_string.replace(
            where_is_it_found,
            size_of_common_escape_charactors, "\t");
          where_is_it_found += size_of_the_replacement_of_escape_char;
        },
        [this](std::string & input_string,
          size_t & where_is_it_found) -> void {
          std::string name = common_functions::read_identifier(this -> line_stream);
          std::string the_nested_non_term_entry_pattern = all_entries.get_pattern_of_nested_non_term_symbol_pattern(name);
          std::string pattern_corrresponding_to_nested_name = "(" + the_nested_non_term_entry_pattern + ")";
          input_string.replace(
            where_is_it_found,
            size_of_common_escape_charactors, pattern_corrresponding_to_nested_name);
          all_entries.add_nested_non_term_symbol_to_the_newest_entry(name);

          where_is_it_found += pattern_corrresponding_to_nested_name.length();

        }

      });
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
  }
  std::istringstream & current_line() {
    return line_stream;
  }
  void get_and_parse_input() {
    do {
      try {
        change_current_line();
        parse_raw_input();

      } catch (const std::ios_base::failure & e) {
        std::cerr << "File stream error: " << e.what() << "\n" << current_line_number << "\n\n\n";
        return;
      } catch (const std::runtime_error & e) {
        std::cerr << "Runtime error: " << e.what() << "\n" << current_line_number << "\n\n\n";

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
