#include<iostream>
import base_class_for_all_parts_of_compiler_generator;
int main() {
  //test:
    Base_class lexer {
    std::cin, std::cout
  };
  lexer.get_and_parse_input();
  lexer.get_and_parse_input();
  lexer.print_all_parsed_input_for_testing();
  return 0;
}
