module;
#include <string>       // For std::string parameters
#include <fstream>      // For std::ifstream and std::ofstream
#include <memory>       // For std::shared_ptr and std::unique_ptr
#include <stdexcept>    // For std::runtime_error
#include <utility>      // For std::move
#include <thread>
// I used AI to keep track of the headers needed

export module Driver;
import Inputer;
import Printer;
import Config_parser;
import absolute_base;

template<absolute_base::Streamable File_Stream_type>
File_Stream_type* file_stream_intializater(std::string input_file_name)  {
	File_Stream_type* input_file_stream = new File_Stream_type{ input_file_name };
		if (!input_file_stream.is_open()) {
			throw std::string{"file_stream named \""+input_file_name+"\"failed to open"}
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
            Printer&& b, bool c, std::string d, bool e)
            :input_file{ std::move(a) },
            output_file{ std::move(b) }, debug_mode{ c }, error_log_stream{ d }, multi_threaded_launch{ e }{
        }
        Input_reader input_file;
        Printer output_file;
        std::ostream error_log_stream;
        bool debug_mode;
		bool multi_threaded_launch;
    public:
        enum class index_from_the_end_of_each_argument_of_single_engine {
            debug_mode = 1,//last argument, the ones below it are the one before it
            debug_mode_for_reading_output_config = 2,
            debug_mode_for_reading_input_config = 3,
            error_log_file = 4,
            error_log_file_for_config = 5,
            output_file = 6,
            input_file = 7,
            output_config_file = 8,
            input_config_file = 9,
            multi_threaded_launch = 10;
            run_engine_optimized_using_multi_threads = 11;
        };
        static Driver_engine create_driver(std::string input_config_file, std::string output_config_file, std::string input_file, std::string output_file, std::string error_log_file_for_config, std::string error_log_file, bool debug_mode_for_reading_input_config, bool debug_mode_for_reading_output_config, bool debug_mode, bool multi_threaded_launch, bool multi_thread_optimization_paremeter) {
            std::ostream error_log_stream_for_config{ error_log_file_for_config };
            try {

                using All_entries = absolute_base::All_non_terminal_entries;

                Config_reader read_input_stream = Config_reader{ file_stream_intializater<std::ifstream>(input_config_file) };
                std::shared_ptr<All_entries> input_config = std::make_shared<All_entries>();
                std::async(std::launch::async, [] {
                    while (debug_mode_for_reading_intput_config) {
                        error_log_stream_for_config << "\n###INPUT CONFIG ERRORS###\n";
                        try {
                            read_input_stream.get_and_parse_input();
                            input_config = std::make_shared<All_entries>(std::move(read_input_stream.get_all_entries_by_l_reference()));
                            break;

                        }
                        catch (std::string error) {
                            error_log_stream_for_config << error << "\n";

                        }
                    }
                    };
                //as you can guess input config is shared because output needs to manage it(if the user asks by providing options) while input needs to use it
                Config_reader output_config_stream = Config_reader{ file_stream_intializater<std::ofstream>(output_config_file) };
                std::unique_ptr<All_entries> output_config = std::make_unique<All_entries>();
                std::async(std::launch::async, [] {

                    while (debug_mode_for_reading_output_config) {
                        error_log_stream_for_config << "\n###OUTPUT CONFIG ERRORS###\n";
                        try {
                            output_config_stream.get_and_parse_input();

                            output_config = std::make_unique<All_entries>(std::move(output_config_stream.get_all_entries_by_l_reference()));
                            break;
                        }
                        catch (std::string error) {
                            error_log_stream_for_config << error << "\n";

                        }

                    }};
                //output isnt shared and is only used by Printer class that both uses it and manages it(if the user asks by providing options)
                using Input_stream_handler_ptr = absolute_base::Streamable_manager<std::ifstream, std::shared_ptr>;
                using Output_stream_handler_ptr = absolute_base::Streamable_manager<std::ofstream, std::unique_ptr>;
                //inner and outer means child and parent pointer respectively
                auto inner_input{ std::make_shared<std::ifstream>(std::move(file_stream_intializater<std::ifstream>(input_file))) };
                auto outer_input{ std::make_shared<std::shared_ptr<std::ifstream>>(inner_input) };
                Input_stream_handler_ptr input{ outer_input };

                auto inner_output{ std::make_unique<std::ofstream>(std::move(file_stream_intializater<std::ofstream>(output_file))) };
                auto outer_output{ std::make_unique<std::unique_ptr<std::ofstream>>(std::move(inner_output)) };
                Output_stream_handler_ptr output{ std::move(outer_output) };
                std::shared_ptr<bool> multi_thread_optimization = std::make_shared<bool>(multi_thread_optimization_paremeter);

                printing_tools::Printer output_printer{ std::move(output), std::move(output_config), input_config, input,multi_thread_optimization };
                //notice i am moving output and its config, that is because only output_printer will need it and because they are unique_ptr's
                input_tools::Input_reader input_reader{ input, output, input_config };
                return Driver_engine{std::move(input_reader), std::move(output_printer), debug_mode, error_log_file,multi_threaded_launch };
            }
            catch (...) {
                error_log_stream_for_config << "DRIVER MODULE CONFIG READER: unexpected error (its basically FBAR(from saving private ryan) this time)\n";
            }
        }
    };
	void run_engine_helper(){
        bool init = true;
        while (debug_mode) {
            try {
                Input_reader.scan_family_tree_regex_pattern_of_root(init);
                init = false;
                break;
            }
            catch (std::string error) {
                error_log_stream << error << "\n";

            }
            catch (...) {
                error_log_stream << "DRIVER MODULE ENGINE: unexpected error (its basically FUBAR(from saving private ryan) this time)\n";
            }
            
    }
}
    void run_engine() {

        if (multi_threaded_launch) {
            std::jthread t([] {] {output_manager.print(match_info[0].str()); });
        }
        else {
            run_engine_helper();
        }
    }
}
