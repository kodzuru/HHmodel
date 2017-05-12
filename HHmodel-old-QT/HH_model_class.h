#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>


class HH_model_class
{
private:
	std::vector<double> parameters;
	std::vector<double> start_point;
	std::vector<std::vector<double> > result_full;
	std::vector<double> result_vi;
	std::vector<double> result_n;
	std::vector<double> result_m;
	std::vector<double> result_h;
    std::vector<int> channel_on_off;
    double time_end;

    double temp_param_7;

    std::vector<double> result_current_K;
    std::vector<double> result_current_Na;
    std::vector<double> result_current_L;

	std::vector<double> Vm(std::vector<double>); //main function
	double alpha_n(double V);
	double beta_n(double V);
	double alpha_m(double V);
	double beta_m(double V);
	double alpha_h(double V);
	double beta_h(double V);
	void ODE_solver(double time_start, double time_end, std::vector<double> Y, int iteration);//solver runge-kutt 4

    void current_K();
    void current_Na();
    void current_L();



public:
    //HH_model_class(параметры, начальные условия, длительность общая, вкл/выкл каналов)
    HH_model_class(std::vector<double>, std::vector<double>,std::vector<double>, std::vector<int>);
	virtual ~HH_model_class();
	std::vector<double> return_result_vi() const;
	std::vector<double> return_result_n() const;
	std::vector<double> return_result_m() const;
	std::vector<double> return_result_h() const;
	std::vector<std::vector<double> > return_result_full() const;

    std::vector<double> return_result_current_K() const;
    std::vector<double> return_result_current_Na() const;
    std::vector<double> return_result_current_L() const;
};

