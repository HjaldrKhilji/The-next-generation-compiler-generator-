#include<iostream>
#include<string>
#include <fstream>

import Inputer;
import Printer;
import Driver

int main(int argc, char* argv[]) {
	if(argc!=6){
	std::cerr<<"proper number of file arguments not passed, the first argument is input config file name, second is output config, third is input, forth is output file name, and last is the file to report all the errors to "<<std::endl;
	}
  	driver::Driver_engine engine=driver::Driver_engine (argv[1], argv[2], argv[3],argv[4]);
	// line to be added in the driver module's runner function engine.parse_raw_input();, that function will report all the errors thrown by exceptions to the file argv[5]
	
  return 0;
}

