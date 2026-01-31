module;
#include<vector>
#include<string>
module estd_regex;
import all_declarations;
import Config_parser;
namespace estd {

 namespace escape_functions{
        
         template<typename config>
        inline void escape_double_backslash_by_reading_nested_symbols()(std::spanstream* line_stream,std::string* input_string, std::string::size_type* where_is_it_found,char delimeter,  absolute_base::All_non_terminal_entries<config>* all_entries) {
           constexpr size_t size_of_string_to_replace = 2;
		   
            input_string->replace(
                *where_is_it_found,
                size_of_string_to_replace,
                "\\");
		}

         template<typename config>
        inline void escaping_delimeter()(std::spanstream* line_stream,std::string* input_string, std::string::size_type* where_is_it_found,char delimeter,  absolute_base::All_non_terminal_entries<config>* all_entries) {
           
            constexpr size_t size_of_string_to_replace = 2;


            input_string->replace(
                *where_is_it_found,
                size_of_string_to_replace,
                std::string{delimeter});

        }


         template<typename config>
        inline void escape_backslash_a_by_reading_nested_symbols()(std::spanstream* line_stream,std::string* input_string, std::string::size_type* where_is_it_found,char delimeter,  absolute_base::All_non_terminal_entries<config>* all_entries) {
           
            constexpr size_t size_of_string_to_replace = 2;

            uint64_t name = absolute_base::read_number_from_string_at_a_position<uint64_t>(line_stream);
            all_entries->add_nested_non_term_symbol_to_the_newest_entry(name);
            input_string->replace(
                *where_is_it_found,
                size_of_string_to_replace, std::string{""});


            config_parsing_tools::Config_reader_helper::semantical_analyzer_entry_reader();

        }
        }


	struct processed_string{
    
	std::string string_to_match;
	
	};
	void read_input(std::istream& stream, processed_string* str, char delimeter, char charactor_to_escape_delimeter_with, absolute_base::All_non_terminal_entries<config>* all_non_term_entries){
	stream>>*str;
	common_functions::escape_string(
    str, 
    {"\\\\", "\\"+std::string{charactor_to_escape_delimeter_with}, "\a"},
	{&escape_functions::escape_double_backslash_by_reading_nested_symbols,
     &escape_functions::escaping_delimeter,
     &escape_functions::escape_backslash_a_by_reading_nested_symbols},
	charactor_to_escape_delimeter_with,
	all_non_term_entries
	);
	}
	namespace helper_functions= printing_tools::helper_templates_for_options::helpers_for_arithmetic_options;
	//aliases to change if you want to move away from boost
	struct part_of_regex{
      bool ignore;
      bool optional;
      uint64_t minimum_number_of_time_to_match;
      uint64_t maximum_number_of_times_to_match;
      processed_string string_to_match;
      std::pair<bool, std::string::size_type>match(const std::string &string_to_match_it_in, std::string::size_type starting_position){
      std::string::size_type position=starting_position;
      uint64_t i=0;
      for(; true;i++){
      if(!string_to_match_it_in.find(string_to_match, position)){
       break;
	  }
	  }
      if(maximum_number_of_times_to_match!=0){
           //this outer if statement is to optimize the two checks inside
           //maximum number being 0 means that we wanted it to be ignored
           if(string_to_match_it_in.find(string_to_match, position)&&i!=maximum_number_of_times_to_match){
              return (optional? (true, starting_position):(false, std::string::npos));
	       }
      }
      if(i<= minimum_number_of_time_to_match){
            return (optional? (true, starting_position):(false, std::string::npos));
	  }
      else{
      return (true, position);
	  }
      
	  }
	};
    struct regex_pattern{
    std::vector<part_of_regex> regex_parts;
    std::pair<bool, std::string> regex_search(const std::string& text_to_search_in) {
		 std::string::size_type position=0;
         for(auto &x:regex_parts){
         std::pair<bool, std::string::size_type> result=
         x.match(text_to_search_in, position);
	     if(!result.first()){
         return (false, std::string{});
		 }
         
         }
         return (true, a.substr(0, position));

	}
    bool regex_match(const std::string& text_to_search_in) {
		 std::string::size_type position=0;
         for(auto &x:regex_parts){
         std::pair<bool, std::string::size_type> result=
         x.match(text_to_search_in, position);
	     if(!result.first()){
         return false;
		 }
         
         }
         return true;

	}
	}
	void read_input(std::istream& stream, regex_patterm* str, char delimeter, char charactor_to_escape_delimeter_with, absolute_base::All_non_terminal_entries<config>* all_non_term_entries){
	for(auto &x:pattern){
	stream>>x->ignore;
	stream>>x->optional;
	stream>>x->minimum_number_of_time_to_match;
	stream>>x->maximum_number_of_times_to_match;
	read_input(stream, x->string_to_match, delimeter, charactor_to_escape_delimeter_with, all_non_term_entries);
	
	}
    //a faster (and simpler) alternative to even boost regex
    
	
}
