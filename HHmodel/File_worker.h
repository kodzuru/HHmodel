#pragma once


class File_worker
/*
	file name output template:
	voltage-dd-mm-yy-counter.out
*/
{
private:
	const std::string dir_data; //directory for data save
	std::string settings_file_name; // options filename
	//options structure
	struct settings
	{
		std::string option_name;
		char ch;
		double option_value;
		settings(std::string& _str, char& _ch, double& _exp_c)
			: option_name{ _str }, ch(_ch), option_value(_exp_c) {};
		settings() {};
	}option{};
	std::map<std::string, settings> options_map; // map of all options


	//ifstream for load options
	std::ifstream file_settings_input;

	//read\write settings from file
	void read_settings();


public:
	File_worker(const std::string& _file_settings_input = "file_config.cfg");
	~File_worker();
	//save results to file
	bool save_to_file(std::vector<std::vector<double> >&, const std::string&);
	bool write_settings();
	bool save_parameters_to_file(const std::map<std::string, double>&, const std::string&) const;


};

