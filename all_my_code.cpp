#include <iostream>

#include <map>

#include <regex>

#include <sstream>

#include <string>

#include <vector>
 // the function below expands a nonterminal names
// that contians other non terminal names into  a single regex
// expression by using the nested non terminal names regex expresisons.

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
    for ( ;line_stream.get(c) && (isalnum(c) || c == '_'); identifier += c);
    line_stream.putback(c);
    return identifier;
  
      
  }
}; // namespace common_functions

class Tokenezer {
  public: 
  std::string get_raw_input() {
    std::string raw_input {};
    std::getline( * input_stream, raw_input);
    constexpr size_t size_of_common_escape_charactors = 2;
    constexpr size_t size_of_the_replacement_of_escape_char = 1;
    //provide a escape charactor for \ and for joining the next line into the current one; \\N 
    common_functions::escape_string(
      raw_input, {
        "\\\\",
        "\\N"
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
            "\n" + temp_input);
          where_is_it_found +=
            size_of_the_replacement_of_escape_char + temp_input.length();
        }

      });
    current_line_number++;
    return raw_input;
  }
  void parse_raw_input() {
    constexpr size_t size_of_common_escape_charactors = 2;
    constexpr size_t size_of_the_replacement_of_escape_char = 1;
    std::string non_terminal_name = common_functions::read_identifier(this -> line_stream);
    std::string non_terminal_pattern;
    line_stream >> non_terminal_pattern;
    non_terminal_regex_table[non_terminal_name].push_back("");//adding_initial_sapce
    
    common_functions::escape_string(
      non_terminal_pattern, {
        "\\n",
        "\\S",
        "\\t",
        "\\A"
      }, {
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
        [ &non_terminal_name,& non_terminal_pattern, this](std::string & input_string,
          size_t & where_is_it_found) -> void {
               std::string temp = common_functions::read_identifier(this -> line_stream);
                if (temp.empty()) {
            throw std::runtime_error("non terminal symbol not found. The output after this is undefined!");
              
          }
               temp="("+temp+")";
          input_string.replace(
            where_is_it_found,
            size_of_common_escape_charactors, temp);
            non_terminal_regex_table[non_terminal_name].push_back(temp);
         
          

          non_terminal_pattern += temp;
          where_is_it_found += temp.length();
        }
      });
        non_terminal_regex_table[non_terminal_name][0]= non_terminal_pattern;
  }

  void output_parsed_input_from_all_read_lines_to_output_stream() {
    for (auto symbol_declaration: non_terminal_regex_table) {
  
        //the code below first outputs:
        //1.the non term name, of which the information is gonna be printed for.
        //2.then it prints the non term name's regex pattern.
        //3.then it prints the non term names whose patterns are
        //included as capture groups the current non term pattern.
      * output_stream << symbol_declaration.first << " ";
    std::for_each(begin(symbol_declaration.second),end(symbol_declaration.second),
     [this](std::string non_term_names) { *output_stream << non_term_names<<" ";
        }
    );
    * output_stream <<"\\END"<<std::endl;
  }
      
  }
   
   void change_output_stream(std::ostream *new_output_stream){
       output_stream =new_output_stream;
        is_output_stream_owned=   true;

   }
    void change_input_stream(std::istream *new_input_stream){
       input_stream=new_input_stream;
        is_input_stream_owned =true;
        
    }
     void change_output_stream(std::ostream &new_output_stream){
       output_stream=&new_output_stream;
        is_input_stream_owned=false;
   }
   
   void change_input_stream(std::istream &new_input_stream){
       input_stream=&new_input_stream;
      is_input_stream_owned =false;
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
  
  Tokenezer(std::istream & a, std::ostream & b): input_stream {
    & a
  },
  output_stream {
    & b
  },
  is_input_stream_owned {
    false
  },
  is_output_stream_owned {
    false
  },line_stream{""} {}
  Tokenezer(std::istream * a, std::ostream * b): input_stream {
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
  }, line_stream{""} {}
  ~Tokenezer() {
    if (is_input_stream_owned) delete input_stream;
   
    if (is_output_stream_owned) delete output_stream;
  }

  private:

    int current_line_number;
  std::map < std::string,
  std::vector<std::string> > non_terminal_regex_table;
  std::istringstream line_stream;
  std::istream * input_stream;
  bool is_input_stream_owned;
  std::ostream * output_stream;
  bool is_output_stream_owned;
};

int main() {
    //test:
  Tokenezer lexer {
    std::cin, std::cout
  };
lexer.get_and_parse_input();
lexer.get_and_parse_input();
lexer.output_parsed_input_from_all_read_lines_to_output_stream();
  return 0;
}
