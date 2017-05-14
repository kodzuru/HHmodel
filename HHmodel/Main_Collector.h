#pragma once

class Main_Collector
{
private:
	//exceptions class
	class Invalid : public std::exception
	{
	private:
		const char* message;
	public:
		virtual const char* what() const noexcept {

			return message;
		}
		Invalid(const char* mess) : message{ mess }
		{

		}
	};


	//parameters to solver
	std::vector<double>* parameters = new std::vector<double>;
	std::vector<double>* initial_conditions = new std::vector<double>;
	std::vector<int> *channel_on_off = new std::vector<int>;

	//main solver class
	HH_model_class* HH_model;
	//main file worker class
	File_worker *FW;

	//collect full results vartiables
	std::vector<std::vector<double> > *result_full = new std::vector<std::vector<double> >;
	std::vector<std::vector<double> > *result_current_full = new std::vector<std::vector<double> >;
	void voltage_collector();
	void current_collector();

	//parameters map
	std::map<std::string, double> parameters_map;
	std::vector<std::string> paremeters_name{
		"Cm",
		"Gk",
		"Vk",
		"GNa",
		"VNa",
		"Gl",
		"Vl",
		"Stimul_voltage",
		"time_stimul_start",
		"time_stimul_end",
		"full_time_duration",
		"iteration_count",
		"initial_conditions_vi",
		"initial_conditions_n",
		"initial_conditions_m",
		"initial_conditions_h",
		"channel_on_off_K",
		"channel_on_off_Na",
		"channel_on_off_l"
	};
	void parameters_collector();

public:
	Main_Collector(std::vector<double>*, std::vector<double>*, std::vector<int>*);
	Main_Collector();
	~Main_Collector();

	//return results vectors
	std::vector<std::vector<double> >* return_voltage_result();
	std::vector<std::vector<double> >* return_current_result();


};

