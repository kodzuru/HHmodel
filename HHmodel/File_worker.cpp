#include "File_worker.h"



File_worker::File_worker(const std::string& _file_settings_input) :
	file_settings_input{ _file_settings_input }, settings_file_name(_file_settings_input), dir_data("Data/")
{
	read_settings();
}


File_worker::~File_worker()
{
}

void File_worker::read_settings()
{
	//variables for ofstream
	std::string option{};
	char ch{};
	double value{};
	while (file_settings_input >> option >> ch >> value) { //read line as line file
		options_map.insert(std::pair<std::string, settings>(option, settings{ option ,ch,	value }));
		/*
		for (auto it = options_map.begin(); it != options_map.end(); ++it)
		{
			std::cout << (*it).first << " : " << (*it).second.option_value << std::endl;
		}
		*/
	}
}

bool File_worker::write_settings()
{
	//options change
	options_map.find("ExperimentCounter")->second.option_value++;


	if (std::ofstream fout{ settings_file_name }) {
		for (auto it = options_map.begin(); it != options_map.end(); ++it)
		{
			fout << (*it).second.option_name << " "	<< (*it).second.ch << " " << (*it).second.option_value << "\n";
		}
		return true;
	}
	return false;
		
}

bool File_worker::save_to_file(std::vector<std::vector<double> >& result_full, const std::string& input_name)
{
	// local date
	time_t t;
	tm *tk;
	time(&t);
	tk = localtime(&t);
	std::cout << 1900 + tk->tm_year << 1 + tk->tm_mon << tk->tm_mday << std::endl;

	//generate filename
	std::string filename_output{
		input_name + "-"
		+ std::to_string(tk->tm_mday) + "-"
		+ std::to_string(1 + tk->tm_mon) + "-"
		+ std::to_string(1900 + tk->tm_year) + "-"
		+ std::to_string((int)options_map.find("ExperimentCounter")->second.option_value)  //options.Expetiment_counter
		+ ".out" };

	std::string temp_str{};
	if ( std::ofstream fout{ dir_data + filename_output } ) { //save data to file

		for (size_t j(0); j < result_full[0].size(); ++j) { // save vector
			
			for (size_t i(0); i < result_full.size(); ++i) {
				if (i != (result_full.size() - 1)) {
					temp_str += (std::to_string(result_full[i][j]) + "\t");
				}
				else {
					temp_str += (std::to_string(result_full[i][j]) + "\n");
				}
			}
			fout << temp_str;
			temp_str = "";
		}
		std::cout << filename_output << "  success !!!" << std::endl;
		
		return true;
	}
	return false;
}


bool File_worker::save_parameters_to_file(const std::map<std::string, double>& input_map, const std::string& input_name) const
{
	// local date
	time_t t;
	tm *tk;
	time(&t);
	tk = localtime(&t);
	std::cout << 1900 + tk->tm_year << 1 + tk->tm_mon << tk->tm_mday << std::endl;

	//generate filename
	std::string filename_output{
		input_name + "-"
		+ std::to_string(tk->tm_mday) + "-"
		+ std::to_string(1 + tk->tm_mon) + "-"
		+ std::to_string(1900 + tk->tm_year) + "-"
		+ std::to_string((int)options_map.find("ExperimentCounter")->second.option_value)  //options.Expetiment_counter
		+ ".out" };

	if (std::ofstream fout{ dir_data + filename_output }) {
		for (auto it = input_map.begin(); it != input_map.end(); ++it)
		{
			fout << (*it).first << " = " << (*it).second << "\n";
		}
		return true;
	}
	return false;
}