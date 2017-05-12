#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <random>
#include <time.h>
#include <map>

#include "HH_model_class.h"
#include "File_worker.h"


int main(int argc, char** argv) {

	srand(time(NULL));

	char ch;
	std::cin >> ch;
	double Cm = 1.0; //param[0]

	double Gk = 36.0;//param[1]
	double Gna = 120.0;//param[3]
	double Gl = 0.3;//param[5]

	double Vk = -77.0;//param[2]
	double Vna = 50.0;//param[4]
	double Vl = -54.387;//param[6]

	double Stimul_voltage = 10.0; // voltage stimul //param[7]
	double time_stimul_start = 1.0; //param[8]
	double time_stimul_end = 3.0;//param[9]
	double full_time_duration = 10.0;//param[10]
	int iteration_count = 1000;//param[10]

	//constant parameter for solver
	std::vector<double>* param = new std::vector<double>;
	param->emplace_back(Cm);
	param->emplace_back(Gk);
	param->emplace_back(Vk);
	param->emplace_back(Gna);
	param->emplace_back(Vna);
	param->emplace_back(Gl);
	param->emplace_back(Vl);

	param->emplace_back(Stimul_voltage);
	param->emplace_back(time_stimul_start);
	param->emplace_back(time_stimul_end);
	param->emplace_back(full_time_duration);

	param->emplace_back(iteration_count);


	//start point for solver
	std::vector<double>* initial_conditions = new std::vector<double>;
	initial_conditions->emplace_back(-64.6489); //start voltage
	initial_conditions->emplace_back(0.3347); //start n
	initial_conditions->emplace_back(0.0677); //start m
	initial_conditions->emplace_back(0.5864); //start h

	//ON\OFF channel
	std::vector<int> *channel_on_off = new std::vector<int>;
	channel_on_off->emplace_back(1);
	channel_on_off->emplace_back(1);
	channel_on_off->emplace_back(1);


	HH_model_class* HH_model = new HH_model_class{ *param, *initial_conditions, *channel_on_off }; // main functionality
	File_worker* FW = new File_worker{}; // class to work with files

	//------------------------------------------------------
	//VOLTAGE
	// result of equation solve
	std::vector<double>* result_vi = new std::vector<double>{ HH_model->return_result_vi() };
	std::vector<double>* result_n = new std::vector<double>{ HH_model->return_result_n() };
	std::vector<double>* result_m = new std::vector<double>{ HH_model->return_result_m() };
	std::vector<double>* result_h = new std::vector<double>{ HH_model->return_result_h() };


	// put results into file
	std::vector<std::vector<double> > *result_full = new std::vector<std::vector<double> >;// = new std::vector<std::vector<double> >;
	result_full->emplace_back(*result_vi);
	result_full->emplace_back(*result_n);
	result_full->emplace_back(*result_m);
	result_full->emplace_back(*result_h);

	//save to file
	if (FW->save_to_file(*result_full, "Voltage")) {
		std::cout << "file successfull saved" << std::endl;
	}
	
	//------------------------------------------------------
	//CURRENT
	std::vector<double>* result_current_K = new std::vector<double>{ HH_model->return_result_current_K() };
	std::vector<double>* result_current_Na = new std::vector<double>{ HH_model->return_result_current_Na() };
	std::vector<double>* result_current_L = new std::vector<double>{ HH_model->return_result_current_L() };
	std::vector<double>* result_current_sum = new std::vector<double>{ HH_model->return_result_current_sum() };
	
	// put results into file
	std::vector<std::vector<double> > *result_current_full = new std::vector<std::vector<double> >;// = new std::vector<std::vector<double> >;
	result_current_full->emplace_back(*result_current_sum);
	result_current_full->emplace_back(*result_current_K);
	result_current_full->emplace_back(*result_current_Na);
	result_current_full->emplace_back(*result_current_L);
	
	//save to file
	if (FW->save_to_file(*result_current_full, "Current")) {
		std::cout << "file successfull saved" << std::endl;
	}




	
	//------------------------------------------------------
	//PARAMETERS
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

	//make parameters vector of structure
	size_t parameters_name_counter = 0;
	for (auto it = param->begin(); it != param->end(); ++it) {
		parameters_map.insert(std::pair<std::string, double>(paremeters_name[parameters_name_counter++], *it));

		if (parameters_name_counter == param->size()-1) {
			for (auto it_1 = initial_conditions->begin(); it_1 != initial_conditions->end(); ++it_1) {
				parameters_map.insert(std::pair<std::string, double>(paremeters_name[parameters_name_counter++], *it_1));
				if (parameters_name_counter == (param->size() + initial_conditions->size() - 2)) {
					for (auto it_2 = channel_on_off->begin(); it_2 != channel_on_off->end(); ++it_2) {
						parameters_map.insert(std::pair<std::string, double>(paremeters_name[parameters_name_counter++], *it_2));
					}
				}
			}
		}
	}
	parameters_map.insert(std::pair<std::string, double>(std::string{"ODE_error_mean"}, HH_model->ODE_error_mean()));

	if (FW->save_parameters_to_file(parameters_map, "params")) {
		std::cout << "parameters successfull saved!!!" << std::endl;
	}
	else {
		std::cout << "parameters ERROR saved!!!" << std::endl;
	}

	std::cout << std::endl << "Epsilon Mean " << HH_model->ODE_error_mean() << std::endl;
	std::cout << "done!" << std::endl << std::endl;
	



	FW->write_settings(); // save options before quit
	// memory malloc
	delete initial_conditions;
	delete param;
	delete channel_on_off;
	delete HH_model;
	delete result_vi;
	delete result_n;
	delete result_m;
	delete result_h;
	delete result_full;
	delete result_current_K;
	delete result_current_Na;
	delete result_current_L;
	delete result_current_sum;
	delete result_current_full;
	delete FW;

	_getch();
	return 0;
}


