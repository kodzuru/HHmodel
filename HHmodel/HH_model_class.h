#pragma once


class HH_model_class
{
private:
	//--------------------------------------
	// Parameters to constructor
	std::vector<double> parameters; //+
	std::vector<double> start_point; //+
	std::vector<int> channel_on_off; //+

	//--------------------------------------
	//temp parameter for stimul voltage
	double temp_param_for_stimul_voltage;

	//--------------------------------------
	//return result variables
	std::vector<double> result_vi;
	std::vector<double> result_n;
	std::vector<double> result_m;
	std::vector<double> result_h;


	std::vector<double> result_current_K;
	std::vector<double> result_current_Na;
	std::vector<double> result_current_L;
	std::vector<double> result_current_sum;



	//---------------------------------------
	//main solver functon
	void ODE_solver(double time_start, const double& time_end, std::vector<double>& Y, const int& iteration);//solver runge-kutt 4

	std::vector<double> Vm(std::vector<double>); //main function
	double alpha_n(const double& V) const;
	double beta_n(const double& V) const;
	double alpha_m(const double& V) const;
	double beta_m(const double& V) const;
	double alpha_h(const double& V) const;
	double beta_h(const double& V) const;

	void calculate_result_current_K();
	void calculate_result_current_Na();
	void calculate_result_current_L();
	void calculate_result_current_sum();



public:
	//using DataType_vec_d = std::vector<double>;
	HH_model_class(std::vector<double>&, std::vector<double>&, std::vector<int>&);
	virtual ~HH_model_class();
	std::vector<double> return_result_vi() const;
	std::vector<double> return_result_n() const;
	std::vector<double> return_result_m() const;
	std::vector<double> return_result_h() const;

	std::vector<double> return_result_current_K() const;
	std::vector<double> return_result_current_Na() const;
	std::vector<double> return_result_current_L() const;
	std::vector<double> return_result_current_sum() const;

	//std::vector<std::vector<double> > return_result_current_full() const;

	double ODE_error_mean();
};

