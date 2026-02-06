#include <iostream>  // For std::cerr and std::endl
#include <string>    // For argv processing (if needed by your engine)
// I used AI to keep track of the headers needed

import Inputer;
import Printer;
import Driver
import All_declarations;


int main(int argc, char* argv[]) {
	using index = driver::index_from_the_end_of_each_argument_of_single_engine;
	if(argc!=14){
	std::cerr<<"Minimum of 14 arguments required to start"<<std::endl;
	}
	else if (!(argc % 14)) {
		if(argc % 14==index::error_log_file_for_config){
		std::ostream{ argv[argc - index::error_log_file_for_config] } << "each engine instance needs 14 arguments";
		}
		std::cerr << "each engine instance needs 14 arguments" << std::endl;
	}
	else {
		int currently_to_be_processed_size = 14;

		do {
			
			driver::Driver_engine engine = driver::create_driver(
				argv[currently_to_be_processed_size - index::debug_mode],
				argv[currently_to_be_processed_size - index::debug_mode_for_reading_output_config],
				argv[currently_to_be_processed_size - index::debug_mode_for_reading_input_config],
				argv[currently_to_be_processed_size - index::error_log_file],
				argv[currently_to_be_processed_size - index::error_log_file_for_config],
				argv[currently_to_be_processed_size - index::output_file],
				absolute_base::convert_to_bool(argv[currently_to_be_processed_size - index::input_file]),
				absolute_base::convert_to_bool(argv[currently_to_be_processed_size - index::output_config_file]),
				absolute_base::convert_to_bool(argv[currently_to_be_processed_size - index::input_config_file]),
				absolute_base::convert_to_bool(argv[currently_to_be_processed_size - index::multi_threaded_launch]),
				absolute_base::convert_to_bool(argv[currently_to_be_processed_size - index::run_engine_optimized_using_multi_threads]),
				absolute_base::convert_to_char(argv[currently_to_be_processed_size - index::default_input_config_delimeter]),
				absolute_base::convert_to_char(argv[currently_to_be_processed_size - index::default_output_config_delimeter]),
				absolute_base::convert_to_char(argv[currently_to_be_processed_size - index::input_delimeter])

			);
			engine.run_engine();
		
		} while (current_to_be_processed_size<=argc);

	}
  
	
  return 0;
}


