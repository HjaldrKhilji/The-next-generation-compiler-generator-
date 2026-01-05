
module;
#include <iostream>

#include<functional>

#include <map>


#include <sstream>

#include <string>

#include <vector>
#include <climits>
export module Config_parser;

import All_declarations;

export   namespace config_parsing_tools {

    namespace Config_reader_helper {
   
        class Function_object_to_escape_escape_charactors { //lambda has to be used with common_functions::escape_string() function
        public:
            std::string string_to_be_replaced_with;
            size_t size_of_the_replacement_of_escape_string;

            size_t size_of_escape_charactor;
            Function_object_to_escape_escape_charactors(std::string y, size_t x) : string_to_be_replaced_with{
              y
            },
                size_of_the_replacement_of_escape_string{
                  y.length()
            },
                size_of_escape_charactor{
                  x
            } {
            }
            void operator()(std::string* input_string, size_t* where_is_it_found) {
                input_string->replace(*where_is_it_found,
                    size_of_escape_charactor, string_to_be_replaced_with);
                *where_is_it_found += size_of_the_replacement_of_escape_string;
            }
        };



         void escape_backslash_capital_n(std::spanstream* line_stream,std::string* input_string, size_t* where_is_it_found) {
            // Precondition 1: The input stream named line_stream is in a valid state for reading.
         // Precondition 2: `input_string` is a valid string object.
         // Precondition 3: `where_is_it_found` is a valid index within `input_string`
         //                 and marks the start of the substring to be replaced.
         //
         // Postcondition:
         // 2. The substring of `input_string` beginning at `where_is_it_found`
         //    has been replaced by the content of the line read from the stream.
         // 3. If the line read was empty, the substring has been replaced by an
         //    empty string, effectively deleting it.
            constexpr size_t size_of_common_escape_charactors = 2;

            std::string temp_input = get_raw_input(line_stream);

            input_string->replace(
                *where_is_it_found,
                size_of_common_escape_charactors,
                temp_input);

        };

        //used for nested non term regex expression construction:

         void escape_backslash_capital_a_by_reading_nested_symbols(std::spanstream* line_stream, absolute_base::All_non_terminal_entries* all_entries,std::string* input_string, size_t* where_is_it_found) {
            // Precondition 1: The input stream named line_stream is in a valid state for reading.
           // Precondition 2: The non-terminal entry name read from the stream
           // must correspond to an entry that already exists in the global non-terminal
           // entry table.
           // Precondition 3: `input_string` is a valid string object.

           // Postcondition:

           // 2. A new entry for the nested non-terminal, including its associated
           //    semantic rules, has been added to the nested non-terminal list
           //    of the parent non-terminal entry.
           // 3. The newly added semantic rules are local to the context of the
           //    nested non-terminal within this specific parent, and do not affect
           //    the semantic rules of the same non-terminal name in other contexts.
           // 4. The line_stream is positioned immediately after the last
           //    semantic rule read for the nested non-terminal.
            constexpr size_t size_of_common_escape_charactors = 2;

            std::string name = absolute_base::read_identifier(line_stream);
            all_entries->add_nested_non_term_symbol_to_the_newest_entry(name);
            input_string->replace(
                *where_is_it_found,
                size_of_common_escape_charactors, "");


            semantical_analyzer_entry_reader();

        }
        //used ( as of 11/18/2025 only ) for semantic rules regex expression construction:
         void escape_backslash_capital_u_by_reading_nested_symbols(std::spanstream* line_stream, absolute_base::All_non_terminal_entries* all_entries,std::string* input_string, size_t* where_is_it_found) {
            // Precondition 1: The input stream named line_stream is in a valid state for reading.
              // Precondition 2: The non-terminal entry name read from the stream
                  // must correspond to an entry that already exists in the global non-terminal
           // entry table.
                // Precondition 3: `input_string` is a valid string object.
            // Precondition 4: `where_is_it_found` is a valid index within `input_string`
            //                 and marks the start of the substring to be replaced.
              // must correspond to an entry that already exists in the global non-terminal
              // entry table.
              // Postcondition:
              // 1. The input_string has been modified to incorporate the pattern
              //    of the newly processed nested non-terminal.
              // 2. The line_stream is positioned immediately after the last
              //    semantic rule read for the nested non-terminal.
            constexpr size_t size_of_common_escape_charactors = 2;

            std::string name = absolute_base::read_identifier(line_stream);
            std::string& the_nested_non_term_entry_pattern = all_entries->get_pattern_of_nested_non_term_symbol_pattern(name);
            input_string->replace(
                *where_is_it_found,
                size_of_common_escape_charactors, the_nested_non_term_entry_pattern);

            where_is_it_found += the_nested_non_term_entry_pattern.length();

        }
         std::string return_raw_config_for_pattern(std::spanstream* line_stream) {
            // Precondition: `line_stream` is in a valid state for reading.
        //
        // Postcondition:
        // 1. The function returns a string derived from the next word
        //    read from `line_stream`.
        // 2. The returned string has had certain special pattern characters
        //    escaped according to the following rules:
        //    - The character '+' is replaced by the string "{1,UINT_MAX}".
        //    - The character '*' is replaced by the string "{0,UINT_MAX}".
        //    - The character '?' is replaced by the string "{0,1}".
        // 3. The line_stream is advanced past the word that was read.
            constexpr int size_of_common_escape_charactors = 1;
            std::string raw_config;
            *line_stream >> std::skipws >> raw_config;
            absolute_base::escape_string(
                &raw_config, {
                  "+",
                  "*",
                  "?"

                }, {
                  Function_object_to_escape_escape_charactors {
                    "{1,UINT_MAX}",
                    size_of_common_escape_charactors
                  },
                  Function_object_to_escape_escape_charactors {
                    "{0,UINT_MAX}",
                    size_of_common_escape_charactors
                  },
                  Function_object_to_escape_escape_charactors {
                    "{0,1}",
                    size_of_common_escape_charactors
                  }
                }
            );

            return raw_config;
        }

         std::string take_space_terminated_input_and_escape_it(std::spanstream* line_stream, absolute_base::All_non_terminal_entries* all_entries) {
            // Precondition:
        // 1. `line_stream` is in a valid state for reading.
        // 2. The necessary helper functions for escaping (`escape_backslash_capital_n`, `escape_backslash_capital_u_by_reading_nested_symbols`) are available and correctly implemented.
        // 3. Any input sequence in the stream that contains '\\U' must be followed by
        //    a correctly formatted nested symbol,  after the first space, that occurs after the sequence.
        //    A nested symbol is defined as a sequence of characters enclosed in
        //    balanced parentheses `()`. For example, a literal `(` or `)` within
        //    the nested symbol itself must be escaped with a backslash `\(` or `\)`.
        // Postcondition:
        // 1. A space-terminated string is read from `line_stream`.
        // 2. The returned value is that string, enclosed in parentheses, with specific characters escaped.
        // 3. The following escape rules are applied to the string before it is enclosed in parentheses:
        //    - The sequence '\\' is replaced by a single backslash '\'.
        //    - The sequence '\S' is replaced by a space ' '.
        //    - The sequence '\t' is replaced by a tab character.
        //    - The sequence '\n' is replaced by a newline character.
        //    - The character '(' is escaped to '\('.
        //    - The character ')' is escaped to '\)'.
        //    - The sequence '\U' is replaced by the result of `escape_backslash_capital_u_by_reading_nested_symbols`.
        //    - The sequence '\N' is replaced by the result of `escape_backslash_capital_n`.
        // 4. `line_stream` is advanced past the word that was read.
        // 5. If any of the escape operations result in a read from the stream (e.g., `\U`), the stream is further advanced according to the implementation of \U

          ///post condition : line_stream argument's state is changed to have the property "std::skipws" set.

            constexpr size_t size_of_common_escape_charactors = 2;
            std::string semantic_pattern_to_check;
            *line_stream >> std::skipws >> semantic_pattern_to_check;
            absolute_base::escape_string(
                &semantic_pattern_to_check, {
                  "\\\\",
                  "\\S",
                  "\\t",
                  "\\n",
                  "(",
                  ")",
                  "\\U",
                  "\\N"

                }, {
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
                    "\\(",
                    1
                  } //size of ( is one hence I wrote 1 in the second argument
                  ,
                  Function_object_to_escape_escape_charactors {
                    "\\)",
                    1
                  },
                  [line_stream,all_entries](std::string& input_string, size_t& where_found) {
                    escape_backslash_capital_n(line_stream, &input_string, &where_found);
                  },
                  [line_stream, all_entries](std::string& input_string, size_t& where_found) {
                    escape_backslash_capital_u_by_reading_nested_symbols(line_stream, all_entries,&input_string, &where_found);
                  }
                //size of ) is one hence I wrote 1 in the second argument
                });

                return "(" + std::move(semantic_pattern_to_check) + ")";
        }

         void parse_config_for_semantic_entry(std::spanstream* line_stream,absolute_base::Settings_for_semantical_rules* settings_for_current_config, unsigned int* minimum_amount_of_Matches, unsigned int* maximum_amount_of_matches) {
            //precondition: settings_for_current_config, line_stream,minimum_amount_of_Matches reference, and maximum_amount_of_matches are an references to objects
            //  that actually exists
            //precondition: line_stream has to be in readable condition(if it has been tempered with(by reading) before and then passed as a rvalue reference using std::move())
            //precondition: settings_for_current_config has Settings_for_semantical_rules::check_exist and Settings_for_semantical_rules::check_atleast bits set
            //postcondition: settings_for_current_config is contains all the settings inputed by the input_stream as follows:
            //if the line_stream contained ! then the Settings_for_semantical_rules::check_exist is unset and Settings_for_semantical_rules::check_dont_exist bit is set
            //if the line_stream contained , then the Settings_for_semantical_rules::check_atleast is unset and Settings_for_semantical_rules::check_exact bit is set
            //postcondition: minimum_amount_of_Matches contains the minimum amount of matches if semantical_rules::check_atleast is set, else contains exact matches amount number
            //postcondition: maximum_amount_of_matches contains the maximum amount of matches if  semantical_rules::check_atleast  is set, else is undefined
            //post condition: the line stream is now considered to be fully read
            using absolute_base::Settings_for_semantical_rules;
            char parse_config_one_by_one;

            *line_stream >> parse_config_one_by_one;
            switch (parse_config_one_by_one) {
            case '!':
                *settings_for_current_config |= Settings_for_semantical_rules::check_dont_exist;
                *settings_for_current_config ^= Settings_for_semantical_rules::check_exist;
                *line_stream >> parse_config_one_by_one;

            case '{':
                *line_stream >> *minimum_amount_of_Matches;
                *line_stream >> parse_config_one_by_one;

            case ',':
                *line_stream >> parse_config_one_by_one;
                *settings_for_current_config |= Settings_for_semantical_rules::check_exact;
                *settings_for_current_config ^= Settings_for_semantical_rules::check_atleast;
                *line_stream >> *maximum_amount_of_matches;

            case '}':
                break;
            default:
                std::cerr << "invalid configuration input, proceed on your own risk" << std::endl;
                throw std::runtime_error("parse_config_for_semantic_entry error");
                break;
            }

        }

         absolute_base::Semantical_analyzer_config_entry return_semantical_analyzer_entry(std::spanstream* line_stream, absolute_base::All_non_terminal_entries* all_entries) {
            //precondition: the inputstream named line_stream is in readable state
            //precondition: the line_stream is either empty or contains data in a valid format for reading semantic configuration
            //the valid format is: a identifier that is not empty, and ends with a space, or a non alphanumeric character or the _ character,
            //followed by a regex pattern that is terminates by a space, and then at the end has a config information in the format:
            //{n} for exactly n amount of matches, or {n,m} for a range of n-m matches, or a short hand as *(will be evaluated as {0, UINT_MAX}), + (will be evaluated as {1, UINT_MAX}), 
            // ? (will be evaluated as {0,1})
            //post condition: semantical entry is read in the format described by the requirement of the user input format in the precondition
            //post condition : line_stream arguments state is changed to have the property std::skipws set.
            //and then the entry is inputed as the newest semantic entry for the nested nested non terminal entry of the newest non terminal entry
              //the regex pattern may contain escape charactors, but they are however obviously escaped, to get to know how they work, go to the function:
              //take_space_terminated_input_and_escape_it, and look for the function call of  common_functions::escape_string()
                  //post condition: the line stream is forwarded after the input of the specific format described in the preconditions is completed


            char c;
            if (!(*line_stream >> std::skipws >> c)) {

                return;
            }
            line_stream->putback(c);
            std::string non_terminal_name_to_search_inside = absolute_base::read_identifier(&line_stream);
            std::string semantic_pattern_to_check = take_space_terminated_input_and_escape_it(line_stream, all_entries);
            unsigned int minimum_amount_of_Matches = 0;
            unsigned int maximum_amount_of_matches = 0; //only used if settings_for_semantic_rules dosent have check_atleast on.
            
            using absolute_base::Settings_for_semantical_rules;
            Settings_for_semantical_rules fully_parsed_config = Settings_for_semantical_rules::check_exist | Settings_for_semantical_rules::check_atleast;
            //notice by default check_exist and check_atleast are turned on
            *line_stream<< return_raw_config_for_pattern(line_stream);
            parse_config_for_semantic_entry(line_stream,&fully_parsed_config, &minimum_amount_of_Matches, &maximum_amount_of_matches);
            
            return absolute_base::Semantical_analyzer_config_entry{
              all_entries.get_parmenant_name_of_nested_non_term_symbol_pattern(non_terminal_name_to_search_inside),
                semantic_pattern_to_check,
                fully_parsed_config,
                minimum_amount_of_Matches,
                maximum_amount_of_matches
            };
            //this function can be used in Printer.ixx to help the user insert a new semantic entry to a non_term entry
        }
         inline absolute_base::Semantical_analyzer_config_entry semantical_analyzer_entry_reader(std::spanstream* line_stream, absolute_base::All_non_terminal_entries* all_entries) {

             all_entries->add_semantic_rule_for_newest_sub_entry(return_semantical_analyzer_entry(line_stream, all_entries));

         }
         std::string get_raw_input(std::istream* input_stream) {
            //precondition: valid state of the input stream named input_stream
            //post condition, return input exactly as it was outputed, and increment current_line_number
            std::string raw_input{};
            std::getline(*input_stream, raw_input);

            return raw_input;
        }

         void push_latest_entry_as_sub_entry_of_an_entry(absolute_base::All_non_terminal_entries* all_entries,  absolute_base::Non_terminal_name_entry* entry_to_push_it_into) {
            
            all_entries->add_a_child_to_entry(entry_to_push_it_into, *(all_entries->physical_end()));

        }
         void parse_raw_input(absolute_base::All_non_terminal_entries* all_entries, std::spanstream *line_stream) {

            //precondition: the inputstream named line_stream is in readable state
            //precondition: the inputstream is either empty or contains data in a valid format for reading a new non term name entry
            //the valid format is: a identifier that is not empty, and ends with a space, or a non alphanumeric character or the _ character,
              //followed by a regex pattern that is terminates by a space, and this can be followed by additional nested entry entries, and the semantic entries for those
              //nested entries, only if the \A is used in the regex pattern, if \U is used then it may be followed by nested entry entries only
              // (for more information on that check \A and \U, check the functions 
              // escape_backslash_capital_a_by_reading_nested_symbols  and escape_backslash_capital_u_by_reading_nested_symbols)     
              //postcondition: a new non terminal entry is added, along side with its nested entries, and the semantical entries of those nested entries.
              //post condition: the line stream is forwarded after the input of the specific format described in the preconditions is completed
          ///post condition : line_stream arguments state is changed to have the property std::skipws set.

            //the non_terminal_pattern in this function will be a regex expression for input derived class and a custom pattern for output derived class.
            //by custom, I mean, the meaning of that pattern for the output class wil be defined by me 
            constexpr size_t size_of_common_escape_charactors = 2;
            std::string non_terminal_name = absolute_base::read_identifier(line_stream);
            std::string non_terminal_pattern;
            *line_stream >> std::skipws >> non_terminal_pattern;
            all_entries->add_non_term_symbol_name(non_terminal_name);
            //replace \\ with \, and \\N with what ever is in the next line
            //replace \\n with \n(newline),\\S with space, \\t with a tab
            //\\A with the pattern corrsponding to the non terminal name after
            //\\A, and push that name into   current_non_terminal_name_entry
            //notice where_is_it_found variable is updated in each lambda for the next loop
            //to read content after the text replaced.
            absolute_base::escape_string(
                &non_terminal_pattern, {
                  "\\\\",
                  "\\N",
                  "\\n",
                  "\\S",
                  "(",
                  ")",
                  "\\t",
                  "\\A"
                }, {
                  Function_object_to_escape_escape_charactors {
                    "\\",
                    size_of_common_escape_charactors
                  },
                  [](std::string& input_string, size_t& where_found) {
                    escape_backslash_capital_n(line_stream,&input_string, &where_found);
                  }

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
                    "\\(",
                    1
                  } //size of ( is one hence I wrote 1 in the second argument
                  ,
                  Function_object_to_escape_escape_charactors {
                    "\\)",
                    1
                  }, //size of ) is one hence I wrote 1 in the second argument
                  Function_object_to_escape_escape_charactors {
                    "\t",
                    size_of_common_escape_charactors
                  },
                  [ line_stream, all_entries](std::string& input_string, size_t& where_found) {
                    escape_backslash_capital_a_by_reading_nested_symbols(line_stream, all_entries,&input_string, &where_found);
                  }

                }

            );
            all_entries->add_non_term_pattern_for_newest_entry("(" + non_terminal_pattern + ")");
        }
        };
        class Config_reader  {
            //Config_reader is also used for output config parsing, though it was originaly
            // meant only for config parsing for input, the only difference in the use
            // of using it for input, vs using it for output, really, just is,
            // that in input, the patterns storage is used for storing regex patterns,
            // and for output it is used to store Output tokens built on the top of the input
            // taken.
            //this class is supposed to be inherited into two classes to process config information about both input and output symbols
            //invariant 1: the input stream named line_stream is in a state where I can assign a string to it(for it to read that strin as a stream),
            //input stream named input_stream is in a valid state
            //invariant 2:output stream named output_stream is in a valid state
            //invariant 3: all_entries is in a valid state
            //now you may be asking as a noob(like me) that what is the valid state of a data member object, and my answer is that the invariant of that object
            //another defintion might be:The object's valid state is defined by its invariant, which must hold true at the beginning and end of every public method call of that object.
        private:
            


            void change_current_line() {
                //precondition: line_stream is already initialized
                //post_condition: the input for a single non term entry is taken, along side the nested non term entries in that non term entry, and the semantical rules for each
                //non term entry in that non term entry
                //postcondition: line_stream is now considered empty because all its input has been read into the structures where the input belongs.
                line_stream.clear();

                line_stream.str(get_raw_input(&input_stream));
                Config_reader_helper::parse_raw_input(&line_stream, &all_entries);

            }
            std::istringstream& current_line() {
                //I have yet to find a use for this function, but I thought it is important for the future maintainer to just have this very trivial function
                return line_stream;
            }

            void get_and_parse_input() {

                //post_condition: the input for a single non term entry is taken, along side the nested non term entries in that non term entry, and the semantical rules for each
                //non term entry in that non term entry, all exceptions that occurred in doing so have also been handled
                //postcondition: line_stream is now considered empty
                //use get_and_parse_input over change_current_line functin!!!
                do {
                    try {
                        change_current_line();

                    }
                    catch (const std::ios_base::failure& e) {
                        //Todo
                        return;
                    }
                    catch (const std::runtime_error& e) {

                        //Todo

                        return;
                    }
                    //notice an extra flush using std::endl isnt required while using cerr stream in the code above because cerr is flush automatically
                } while (line_stream.good() == true);
                line_stream.clear();
                line_stream.str("");
            } // parse the whole file


        public:

            void print_all_parsed_input_for_testing()  {
                //postcondition: simply prints the information for every non term entry, which contains the non term entries,  nested entries of those non term entries,
                //  and the semantic rules of those nested entries

                all_entries.print_all_content();
            }


            
            Config_reader(std::istream* a) :input_stream(a) { get_and_parse_input(); }
            
            Config_reader(Config_reader&) = default;
            Config_reader(Config_reader&&) = default;
             absolute_base::All_non_terminal_entries& get_all_entries_by_l_reference() const {
                return all_entries;
            }
        private:
            absolute_base::All_non_terminal_entries_implementation all_entries{};
            std::istringstream line_stream{ "" };
            std::unique_ptr<std::istream> input_stream;
           
            
        };
    }  
 
