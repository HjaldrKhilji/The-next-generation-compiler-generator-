module;
#include<vector>
#include<string>
module estd_regex;
namespace estd {
	//aliases to change if you want to move away from boost
	struct part_of_regex{
      bool ignore;
      bool optional;
      uint64_t minimum_number_of_time_to_match;
      uint64_t maximum_number_of_times_to_match;
      std::string string_to_match;
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
    //a faster (and simpler) alternative to even boost regex
    
	
}
