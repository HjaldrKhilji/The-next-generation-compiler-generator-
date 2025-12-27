#include<iostream>
#include<string>
#include <fstream>

import Inputer;
import Printer;
import Config_parser;

template<typename T, typename type_to_stream=std::string>
concept OutputFileStream = requires(T & os, type_to_stream  & s) {
	
	{ os } -> std::same_as<std::ofstream&>;
	{ os<< s } -> std::same_as<std::ostream&>;
	
};
template<typename T, typename type_to_stream = std::string>
concept IntputFileStream = requires(T & is, type_to_stream & s) {

	{ is } -> std::same_as<std::ifstream&>;
	{ is >> s } -> std::same_as<std::istream&>;
	
};

template<typename T, typename type_to_stream = std::string>
concept streamable_File = OutputFileStream<T, type_to_stream> || IntputFileStream<T, type_to_stream>;


template<streamable_File File_Stream_type>
File_Stream_type* file_stream_intializater(std::string input_file_name)  {
	File_Stream_type* input_file_stream = new File_Stream_type{ input_file_name };
		if (!input_file_stream.is_open()) {
			throw std::runtime_error{"file_stream named \""+input_file_name+"\"failed to open"}
		}
		return input_file_stream;
	}
	
namespace driver {

    class Driver_engine {
    private:
        //private constructor to be used by only create_driver
        using Config_reader = config_parsing_tools::Config_reader;
        using Input_reader = input_tools::Input_reader;
        using Printer = printing_tools::Printer;

        Driver_engine(
            Config_reader&& a,
            Input_reader&& b,
            Printer&& c,
            Config_reader&& d)
            :input_config_reader{ std::move(a) },
            input_file{ std::move(b) },
            output_file{ std::move(c) },
            output_config{ std::move(d) } {
        }
        Config_reader input_config_reader;
        Input_reader input_file;
        Config_reader output_config;
        Printer output_file;


        static Driver_engine create_driver(std::string a, std::string b, std::string c, std::string d) {

            using All_non_terminal_entries = absolute_base::All_non_terminal_entries;
            Config_reader input_config =  Config_reader{ file_stream_intializater<std::ifstream>(a) };
            All_non_terminal_entries* All_non_terminal_entries_for_input_config = new All_non_terminal_entries{ std::move(input_config.get_all_entries_by_l_reference()) };

            Config_reader output_config =  Config_reader(file_stream_intializater<std::ifstream>(b));
            All_non_terminal_entries* All_non_terminal_entries_for_input_config = new All_non_terminal_entries{ std::move(input_config.get_all_entries_by_l_reference()) };

            std::ifstream* input =  file_stream_intializater<std::ifstream>(c);
            std::ofstream* output = file_stream_intializater<std::ofstream>(d);

            Printer output_printer = Printer(/* arguments */);
            Input_reader input_reader = Input_reader(
                input,
                output_printer,
                *(config.get_const_all_entries().begin())
            );

            return Driver_engine(std::move(config), std::move(input_reader), std::move(output_config), std::move(output_printer));
        }
    };
}
int main(int argc, char* argv[]) {

	
  return 0;
}

