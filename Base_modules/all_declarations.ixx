module;
#include <iostream>

#include<functional>

#include <map>


#include <sstream>

#include <string>

#include <vector>
#include <deque>
#include<stack>
#include<concepts>
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



my future self: please dont replace istringstream( in other files), until the code is finished to the point that someone will actually use it, and question it for performance, so yeah, I need it!
 
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
    namespace common_functions {
        using escape_charactor_function_wrapper_type = std::function < void(
            std::string& input_string, size_t& where_is_it_found) >;
        void escape_string(std::string& input_string,
            const std::vector < std::string >& strings_to_be_replaced,
            const std::vector < escape_charactor_function_wrapper_type >
            function_to_be_run_for_each);
        std::string read_identifier(std::istringstream& line_stream);
        template <std::integral T>
        std::string::size_type read_integer_from_string_at_a_position(const std::string& source, std::string::size_type position, T* integer_read);

    }
    export namespace absolute_base {

        using common_functions::escape_string;
        using common_functions::read_identifier;
        using common_functions::read_integer_from_string_at_a_position;

        enum class Settings_for_semantical_rules : short {
            NONE = 0,
            check_exist = 1,
            check_dont_exist = 2,
            check_atleast = 4,
            check_exact = 8
        };
        using Underlying_type_of_settings_for_semantical_rules = std::underlying_type_t<Settings_for_semantical_rules>;

      
        //     ~Functions for enum class Settings_for_semantical_rules~ 

        bool operator&&(Settings_for_semantical_rules a, Settings_for_semantical_rules b) {
            using underlying_t = Underlying_type_of_settings_for_semantical_rules;
            return static_cast<bool>(
                static_cast<underlying_t>(a) & static_cast<underlying_t>(b)
                );
        }

        Settings_for_semantical_rules operator|(Settings_for_semantical_rules a, Settings_for_semantical_rules b) {
            using underlying_t = Underlying_type_of_settings_for_semantical_rules;
            return static_cast<Settings_for_semantical_rules>(
                static_cast<underlying_t>(a) | static_cast<underlying_t>(b)
                );
        }

        Settings_for_semantical_rules& operator|=(Settings_for_semantical_rules& a, Settings_for_semantical_rules b) {
            a = a | b;
            return a;
        }
        Settings_for_semantical_rules operator^(Settings_for_semantical_rules a, Settings_for_semantical_rules b) {
            using underlying_t = Underlying_type_of_settings_for_semantical_rules;
            return static_cast<Settings_for_semantical_rules>(
                static_cast<underlying_t>(a) ^ static_cast<underlying_t>(b)
                );
        }

        Settings_for_semantical_rules& operator^=(Settings_for_semantical_rules& a, Settings_for_semantical_rules b) {
            a = a ^ b;
            return a;
        }

        struct Semantical_analyzer_config_entry {
            std::string the_pattern_to_check;
            std::reference_wrapper < std::string > name_of_non_term_symbol_to_check;
            Settings_for_semantical_rules all_settings;

            unsigned int minimum_amount_of_matches;
            unsigned int maximum_amount_of_matches;
            bool check_pattern(std::string text);
        };
        struct Non_terminal_name_entry {
            
            std::string name;
            union {
                std::string pattern;
                std::string output_config_data;
                //keep the type of both as std::string !!!!!
            
            };
            using sub_entry_type = std::vector < std::reference_wrapper < Non_terminal_name_entry >>; 
            sub_entry_type sub_entries;
            using all_semantical_analysis_rules_type = std::vector < std::vector < Semantical_analyzer_config_entry >>;

            all_semantical_analysis_rules_type all_semantical_analysis_rules;
            ~Non_terminal_name_entry() {}//for supressing warnings of implicitly deleted destructor
            Non_terminal_name_entry(const Non_terminal_name_entry&) = default;
            Non_terminal_name_entry(Non_terminal_name_entry&) = default;
            Non_terminal_name_entry(Non_terminal_name_entry&&) = default;//moving, however, wont be so I allowed it
            Non_terminal_name_entry(std::string a, std::string b, sub_entry_type c, all_semantical_analysis_rules_type d) :name{a}, pattern{b}, sub_entries{c}, all_semantical_analysis_rules{d}
            {}
        };
        class Siblings {
        private:
            //keep the data members are lightweight as possible because I am passing them by value in other parts of the code.
            using representation_type = std::reference_wrapper < std::vector < std::reference_wrapper < Non_terminal_name_entry >>>;
            //the default constructor would obviously initialize the members as expected
            representation_type Representation;
            using representation_type_without_reference_wrapper = std::vector < std::reference_wrapper < Non_terminal_name_entry >>;
            using sub_entries_type = std::vector < std::reference_wrapper < Non_terminal_name_entry >>;
            int current_sibling_index;
        public:
            Siblings(representation_type a, int b) : Representation{ a }, current_sibling_index{ b } {}
            representation_type_without_reference_wrapper& operator()() {
                return Representation.get();
            }

            int get_current_sibling_index() {
                return current_sibling_index;
            }
            void set_current_subling_index(int a) {
                current_sibling_index = a;
            }
            Non_terminal_name_entry& get_current_sibling() {
                return (Representation.get())[current_sibling_index].get();
            }
            bool check_if_current_sibling_has_no_children() {
                return get_current_sibling().sub_entries.empty();

            }
            sub_entries_type get_sub_entries_of_the_last_sub_entry_of_current_sibling() {
                return get_current_sibling().sub_entries.back().get().sub_entries;
            }
            std::vector<absolute_base::Semantical_analyzer_config_entry> get_semantic_rules_for_current_sibling() {
                return get_current_sibling().all_semantical_analysis_rules[get_current_sibling_index()];
            }


        };
        void dig_to_the_leaves_of_the_family_tree(Siblings current_generation, std::stack< Siblings>* family_tree) {


            for (; current_generation.check_if_current_sibling_has_no_children() != true;
                current_generation() = current_generation.get_sub_entries_of_the_last_sub_entry_of_current_sibling(),
                current_generation.set_current_subling_index(current_generation.get_sub_entries_of_the_last_sub_entry_of_current_sibling().size() - 1)) {
                (*family_tree).push(Siblings{ current_generation(), current_generation.get_current_sibling_index() - 1 });

            }



        }
        bool semantic_checks(const std::vector < Semantical_analyzer_config_entry >& check_info,  std::string text) {
            
            for (auto x : check_info) {
                if (x.check_pattern(text) == false) {
                    return false;
                }
            }
            return true;
        }
        
        class Base_printer {
        public:
            virtual bool print(std::string string_to_output, const Non_terminal_name_entry& output_config_entry) = 0;
            


        };
        class All_non_terminal_entries {
            //the traversal begin() returns the last element, and end() returns one before the
            //first element, thats because the last entry will be the one that the user of this
            //structure will PROBABLY traverse from.
        public: void add_non_term_symbol_name(std::string name);//this adds the non term symbol only to fast traversal list
              void add_non_term_pattern_for_newest_entry(std::string pattern);//this adds the newest element to the fast search map as well, after it has enter the pattern into the newest element traversal list
              std::string& get_pattern_of_nested_non_term_symbol_pattern(std::string sub_symbol_name);//this gets the pattern for a name, from the map, and the reason I say for nested non term symbol is because it is generally used for finding the symbols for nested non term symbols
              void add_nested_non_term_symbol_to_the_newest_entry(std::string sub_symbol_name);//this just takes a string, which it assumes to be the name of a non term symbol, and finds the non term symbol(from the map), and adds it's reference into the nested non term symbol list of the newest entry
              void add_semantic_rule_for_newest_sub_entry(const Semantical_analyzer_config_entry&& semantical_rule_entry);//this is my favorite(I spend 4 days on making this), so all it does is that it finds the latest entry from the fast traversal list, then finds the latest sub entry in that, and adds this semantic rule entry for that sub entry 
              std::reference_wrapper < std::string > get_parmenant_name_of_nested_non_term_symbol_pattern(std::string sub_symbol_name);//returns the permanent reference to a non term name
              void print_all_content();
              Non_terminal_name_entry& get_current_non_term_entry(int index);//simply gets the non term entry at the given index
              Non_terminal_name_entry& get_current_nested_non_term_entry(int index_for_nested_non_term, int index);//simple gets the nested non term entry at the given indexes, or in [index][index_for_nested_non_term]
              using ReverseIt = std::reverse_iterator<std::deque < Non_terminal_name_entry >::iterator>;

              ReverseIt begin() {
                  return list_of_all_non_term_entries_for_fast_traversal.rbegin();



              };
              ReverseIt end() {
                  return list_of_all_non_term_entries_for_fast_traversal.rend();

              };

        private:
            //this type isnt a Value oriented type, but rather a reference type to a large
            // global "database" of the entire program.
            //the three types and aliases are just implementation details, please ignore it tho, if you arent implementing this class.
            using Reference_to_string = std::reference_wrapper < std::string >;
            using Reference_to_Non_terminal_name_entry = std::reference_wrapper < Non_terminal_name_entry >;
            struct Function_object_class_to_compare_underlying_objects_of_a_reference {
                bool operator()(const std::reference_wrapper < std::string >& a,
                    const std::reference_wrapper < std::string >& b) const {
                    return a.get() < b.get();
                }
            };

            //it should be obvious that the map is for fast lookups, and deque is for fast traversal:
            std::map < Reference_to_string,
                Reference_to_Non_terminal_name_entry,
                Function_object_class_to_compare_underlying_objects_of_a_reference > map_for_fast_retrival_of_entries;
            std::deque < Non_terminal_name_entry > list_of_all_non_term_entries_for_fast_traversal;

        };
        class Base_class_for_all_parts_requiring_input {//this class wont be implemented, but instead inherited by another module that would finally implement it after inheriting the signature.
            //that module will also export this module
        public:


            virtual void print_all_parsed_input_for_testing() = 0;
            virtual void get_and_parse_input() = 0;//fetches and parses all lines using current line, and handle all the exceptions thrown while trying to do so, my advice to the implementor
            //is to define separate functions for each errors, and pass the state needed to fix that error to that function.


        };
    };
