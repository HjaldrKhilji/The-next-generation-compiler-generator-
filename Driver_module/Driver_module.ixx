module;
#include<iostream>
#include<string>
#include <fstream>
export module Driver;
import Inputer;
import Printer;
import Config_parser;
import absolute_base;

template<absolute_base::Streamable File_Stream_type>
File_Stream_type* file_stream_intializater(std::string input_file_name)  {
	File_Stream_type* input_file_stream = new File_Stream_type{ input_file_name };
		if (!input_file_stream.is_open()) {
			throw std::runtime_error{"file_stream named \""+input_file_name+"\"failed to open"}
		}
		return input_file_stream;
	}
	
export namespace driver {
    
    class Driver_engine {
    private:
        //private constructor to be used by only create_driver
        using Config_reader = config_parsing_tools::Config_reader;
        using Input_reader = input_tools::Input_reader;
        using Printer = printing_tools::Printer;

        Driver_engine(
            Input_reader&& a,
            Printer&& b)
            :input_file{ std::move(a) },
            output_file{ std::move(b) } {
        }
        Input_reader input_file;
        Printer output_file;


        static Driver_engine create_driver(std::string input_config_file, std::string output_config_file, std::string input_file, std::string output_file) {

            using All_entries = absolute_base::All_non_terminal_entries;
            Config_reader read_input_config = Config_reader{ file_stream_intializater<std::ifstream>(input_config_file) };

            std::shared_ptr<All_entries> input_config{ std::make_shared<All_entries>(std::move(read_input_config.get_all_entries_by_l_reference())) };
            //as you can guess input config is shared because output needs to manage it(if the user asks by providing options) while input needs to use it
            Config_reader read_output_config = Config_reader{ file_stream_intializater<std::ofstream>(output_config_file) };

            std::unique_ptr<All_entries> output_config{ std::make_unique<All_entries>(std::move(read_output_config.get_all_entries_by_l_reference())) };
            //output isnt shared and is only used by Printer class that both uses it and manages it(if the user asks by providing options)
            using Input_stream_handler_ptr = absolute_base::Streamable_manager<std::ifstream, std::shared_ptr>;
            using Output_stream_handler_ptr = absolute_base::Streamable_manager<std::ofstream, std::unique_ptr>;
            auto inner_input{ std::make_shared<std::ifstream>(std::move(file_stream_intializater<std::ifstream>(input_file))) };
            auto outer_input{ std::make_shared<std::shared_ptr<std::ifstream>>(inner_input) };
            Input_stream_handler_ptr input{ outer_input };

            auto inner_output{ std::make_unique<std::ofstream>(std::move(file_stream_intializater<std::ofstream>(output_file))) };
            auto outer_output { std::make_unique<std::unique_ptr<std::ofstream>>(std::move(inner_output))};
            Output_stream_handler_ptr output{ std::move(outer_output) };
            printing_tools::Printer output_printer{ std::move(output), std::move(output_config), input_config, input };
            //notice i am moving output and its config, that is because only output_printer will need it and because they are unique_ptr's
            input_tools::Input_reader input_reader{ input, output, input_config };
            return Driver_engine( std::move(input_reader), std::move(output_printer));
        }
    };
	void run_engine(Driver_engine engine){
	//for exception handling
}
}
