module;
#include<vector>
#include<string>
module estd_regex;
import all_declarations;
import Config_parser;
namespace estd {

 namespace option_functions{
        using option_function_signature=void(  absolute_base::All_non_terminal_entries <config>*,  std::istream *, char);
        void sub_entry(bsolute_base::All_non_terminal_entries <config>* all_non_term_entries,  std::istream *extra_input, char line_delimeter){
		
		}
		void semantic_entries( absolute_base::All_non_terminal_entries <config>* all_non_term_entries,  std::istream *extra_input, char line_delimeter){
		
		}
		

		std::vector<option_function_signature> options{};


         
        }


	struct processed_string{
    
	std::string string_to_match;
	
	};
	void read_input(config_parsing_tools::line_stream& stream, processed_string* str, char delimeter, char charactor_to_escape_delimeter_with, absolute_base::All_non_terminal_entries <config>* all_non_term_entries,  std::istream *extra_input){
	stream>>*str;
	
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
	void read_input(config_parsing_tools::line_stream& stream, regex_patterm* str, char delimeter, char charactor_to_escape_delimeter_with, absolute_base::All_non_terminal_entries<config>* all_non_term_entries, std::istream *extra_input){
	for(auto &x:pattern){
	stream>>x->ignore;
	stream>>x->optional;
	stream>>x->minimum_number_of_time_to_match;
	stream>>x->maximum_number_of_times_to_match;
	read_input(stream, x->string_to_match, delimeter, charactor_to_escape_delimeter_with, all_non_term_entries, extra_input, current_aka_latest_entry);
	
	}
    //a faster (and simpler) alternative to even boost regex
    
	
}
