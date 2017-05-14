// HHmodel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char** argv) {

	srand(time(0));


	//----------------------------------------------------------
	// input parameters
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

	//parameter for solver
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


	//----------------------------------------------------------
	//initialize main solver loop
	try {

		// solve equations
		Main_Collector* MC = new Main_Collector{ param, initial_conditions, channel_on_off };

		//return resulst
		std::vector<std::vector<double> > *result_full = MC->return_voltage_result();
		std::vector<std::vector<double> > *result_current_full = MC->return_current_result();

	}
	catch (std::exception& e) {
		std::cerr << std::endl << e.what() << std::endl;
	}

	std::cout << std::endl << "done!" << std::endl << std::endl;


	delete param;
	delete initial_conditions;
	delete channel_on_off;


	_getch();
	return 0;
}

