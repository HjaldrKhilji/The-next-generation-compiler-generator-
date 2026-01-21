module;
#include"dependencies\regex_dependencies-src\include\boost\config.hpp"
#include<vector>
#include<string>
module estd_regex;
namespace estd {
	//aliases to change if you want to move away from boost
	
	 using smatch = boost::smatch;
    using regex  = boost::regex;
    using sregex_iterator = boost::sregex_iterator;
	inline bool regex_search(const std::string& a, smatch& b, const regex& c) {
		return boost::regex_search(a, b, c);
	}
	inline bool regex_search(const std::string& a, const regex& c) {
		return boost::regex_search(a, c);
	}
}
