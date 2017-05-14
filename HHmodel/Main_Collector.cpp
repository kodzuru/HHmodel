#include "Main_Collector.h"
#include "stdafx.h"


Main_Collector::Main_Collector()
{
}


Main_Collector::~Main_Collector()
{
	// memory malloc	
	delete initial_conditions;
	delete parameters;
	delete channel_on_off;
	delete HH_model;
	delete result_full;
	delete result_current_full;
	delete FW;
}

Main_Collector::Main_Collector(std::vector<double>* _params, std::vector<double>* _init_cond, std::vector<int>* _chan_on_off) :
	parameters{ _params }, initial_conditions{ _init_cond }, channel_on_off{ _chan_on_off }
{
	//---------------------------------------------------------------
	HH_model = new HH_model_class{ *parameters, *initial_conditions, *channel_on_off }; // main functionality
	FW = new File_worker{}; // class to work with files

	//---------------------------------------------------------------
	voltage_collector();  // collect voltage result full vector
	//save to file
	if (!FW->save_to_file(*result_full, "Voltage")) {
		throw Invalid{ "Can not save voltage parameters to file" };
	}

	//---------------------------------------------------------------
	current_collector(); // collect current result full vector
	//save to file
	if (!FW->save_to_file(*result_current_full, "Current")) {
		throw Invalid{ "Can not current voltage parameters to file" };
	}

	//---------------------------------------------------------------
	parameters_collector(); // collect parameters map
	if (!FW->save_parameters_to_file(parameters_map, "params")) {
		throw Invalid{ "Can not save main parameters to file" };
	}

	//---------------------------------------------------------------
	// save options to file	
	if (!FW->write_settings()) {
		throw Invalid{ "Can not save options to file" };
	}
}



void Main_Collector::voltage_collector()
{
	//------------------------------------------------------
	//VOLTAGE
	// result of equation solve
	std::vector<double>* result_vi = new std::vector<double>{ HH_model->return_result_vi() };
	std::vector<double>* result_n = new std::vector<double>{ HH_model->return_result_n() };
	std::vector<double>* result_m = new std::vector<double>{ HH_model->return_result_m() };
	std::vector<double>* result_h = new std::vector<double>{ HH_model->return_result_h() };


	// put results into file
	result_full->emplace_back(*result_vi);
	result_full->emplace_back(*result_n);
	result_full->emplace_back(*result_m);
	result_full->emplace_back(*result_h);
}

void Main_Collector::current_collector()
{
	//------------------------------------------------------
	//CURRENT
	std::vector<double>* result_current_K = new std::vector<double>{ HH_model->return_result_current_K() };
	std::vector<double>* result_current_Na = new std::vector<double>{ HH_model->return_result_current_Na() };
	std::vector<double>* result_current_L = new std::vector<double>{ HH_model->return_result_current_L() };
	std::vector<double>* result_current_sum = new std::vector<double>{ HH_model->return_result_current_sum() };

	// put results into file
	result_current_full->emplace_back(*result_current_sum);
	result_current_full->emplace_back(*result_current_K);
	result_current_full->emplace_back(*result_current_Na);
	result_current_full->emplace_back(*result_current_L);
}

void Main_Collector::parameters_collector()
{
	//------------------------------------------------------
	//make parameters vector of structure
	size_t parameters_name_counter = 0;
	for (auto it = parameters->begin(); it != parameters->end(); ++it) {
		parameters_map.insert(std::pair<std::string, double>(paremeters_name[parameters_name_counter++], *it));

		if (parameters_name_counter == parameters->size() - 1) {
			for (auto it_1 = initial_conditions->begin(); it_1 != initial_conditions->end(); ++it_1) {

				parameters_map.insert(std::pair<std::string, double>(paremeters_name[parameters_name_counter++], *it_1));
				if (parameters_name_counter == (parameters->size() + initial_conditions->size() - 2)) {

					for (auto it_2 = channel_on_off->begin(); it_2 != channel_on_off->end(); ++it_2) {
						parameters_map.insert(std::pair<std::string, double>(paremeters_name[parameters_name_counter++], *it_2));
					}
				}
			}
		}
	}
	parameters_map.insert(std::pair<std::string, double>(std::string{ "ODE_error_mean" }, HH_model->ODE_error_mean()));
}

std::vector<std::vector<double> >* Main_Collector::return_voltage_result()
{
	return result_full;
}

std::vector<std::vector<double> >* Main_Collector::return_current_result()
{
	return result_current_full;
}
