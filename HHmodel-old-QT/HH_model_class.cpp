#include "HH_model_class.h"

#include <QDebug>

HH_model_class::HH_model_class(std::vector<double> _param,
                               std::vector<double> _start_point,
                               std::vector<double> _time,
                               std::vector<int> _channel_on_off)
    : parameters(_param),
      start_point(_start_point),
      time_end(_time[1]),
      channel_on_off(_channel_on_off)
{

    temp_param_7 = parameters[7];

    ODE_solver(0, time_end, start_point, 1000);
	result_full.push_back(result_vi);
	result_full.push_back(result_n);
	result_full.push_back(result_m);
	result_full.push_back(result_h);

    this->current_K();
    this->current_Na();
    this->current_L();

}


HH_model_class::~HH_model_class()
{
}

//Hodgkin-Huxley model equaton
std::vector<double> HH_model_class::Vm(std::vector<double> y)
{

	std::vector<double> f(4);

    f[0] = -(1 / parameters[0])*(
                channel_on_off[0] * parameters[1] * pow(y[1], 4)* (y[0] - parameters[2])+
            channel_on_off[1] * parameters[3] * pow(y[2], 3)*pow(y[3], 1)*(y[0] - parameters[4]) +
            channel_on_off[2] * parameters[5] * (y[0] - parameters[6])) + parameters[7];

	f[1] = this->alpha_n(y[0])*(1 - y[1]) - this->beta_n(y[0])*y[1];
	f[2] = this->alpha_m(y[0])*(1 - y[2]) - this->beta_m(y[0])*y[2];
	f[3] = this->alpha_h(y[0])*(1 - y[3]) - this->beta_h(y[0])*y[3];
	

	return f;

}

double HH_model_class::alpha_n(double V)
{
	return (0.01*(V + 50.0)) / (1.0 - exp(-(V + 50.0) / 10.0));
}

double HH_model_class::beta_n(double V)
{
	return 0.125*exp(-(V + 60.0) / 80.0);
}


double HH_model_class::alpha_m(double V)
{
	return (0.1*(V + 35.0)) / (1 - exp(-(V + 35.0) / 10.0));
}


double HH_model_class::beta_m(double V)
{
	return 4.0*exp(-(V + 60.0) / 18.0);
}


double HH_model_class::alpha_h(double V)
{
	return 0.07*exp(-(V + 60.0) / 20.0);
}


double HH_model_class::beta_h(double V)
{
	return 1.0 / (1.0 + exp(-(V + 30.0) / 10.0));
}


// solver function runge-kutt 4
void HH_model_class::ODE_solver(double time_start, double time_end, std::vector<double> Y, int iteration) 
{

	std::vector<double> k1(4);
	std::vector<double> k2(4);
	std::vector<double> k3(4);
	std::vector<double> k4(4);

	//interval
	double h = (time_end - time_start) / iteration;
	int j = 1;
	do
	{
        //ограничения стимула по времени
        if(h*j >= parameters[8] || h*j <= parameters[9]){
            parameters[7] = 0.0;
        }else{
            parameters[7] = temp_param_7;
        }


		//1 iteration
		for (int i(0); i < Y.size(); i++)
		{
			k1[i] = h*this->Vm(Y)[i];
			Y[i] = Y[i] + k1[i] / 2;
		}
		//2 iteration
		for (int i(0); i < Y.size(); i++)
		{
			k2[i] = h*this->Vm(Y)[i];
			Y[i] = Y[i] + k2[i] / 2;
		}
		//3 iteration
		for (int i(0); i < Y.size(); i++)
		{
			k3[i] = h*this->Vm(Y)[i];
			Y[i] = Y[i] + k3[i];
		}
		//4 iteration
		for (int i(0); i < Y.size(); i++)
		{
			k4[i] = h*this->Vm(Y)[i];
			Y[i] = Y[i] + (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6; //value for next loop
		}
		//push value to result vector
		result_vi.push_back(Y[0]);
		result_n.push_back(Y[1]);
		result_m.push_back(Y[2]);
		result_h.push_back(Y[3]);
		
		j++;
	} while (j <= iteration);
	
}
//return value from private varables
std::vector<double> HH_model_class::return_result_vi() const
{
	return result_vi;
}
std::vector<double> HH_model_class::return_result_n() const
{
	return result_n;
}
std::vector<double> HH_model_class::return_result_m() const
{
	return result_m;
}
std::vector<double> HH_model_class::return_result_h() const
{
	return result_h;
}
//return vector of full values
std::vector<std::vector<double> > HH_model_class::return_result_full() const
{
	return result_full;
}

//calculate Potassium current
void HH_model_class::current_K()
{
    std::vector<double>::iterator it_vi = result_vi.begin();
    std::vector<double>::iterator it_n = result_n.begin();
    for(it_vi, it_n; it_vi != result_vi.end(), it_n != result_n.end(); ++it_vi, ++it_n)
    {
        result_current_K.push_back(
                    channel_on_off[0] * parameters[1] * pow(*it_n, 4)* (*it_vi - parameters[2])
                );
    }
}
std::vector<double> HH_model_class::return_result_current_K() const
{
    return result_current_K;
}

//calculate Sodium current
void HH_model_class::current_Na()
{
    std::vector<double>::iterator it_vi = result_vi.begin();
    std::vector<double>::iterator it_m = result_m.begin();
    std::vector<double>::iterator it_h = result_h.begin();

    for(it_vi, it_m, it_h;
        it_vi != result_vi.end(), it_m != result_m.end(), it_h != result_h.end();
        ++it_vi, ++it_m, ++it_h)
    {
        result_current_Na.push_back(
                    channel_on_off[1] * parameters[3] * pow(*it_m, 3)*pow(*it_h, 1)*(*it_vi - parameters[4])
                );
    }
}
std::vector<double> HH_model_class::return_result_current_Na() const
{
    return result_current_Na;
}

//calculate Leak current
void HH_model_class::current_L()
{
    std::vector<double>::iterator it_vi = result_vi.begin();
    for(it_vi; it_vi != result_vi.end(); ++it_vi)
    {
        result_current_L.push_back(
                    channel_on_off[2] * parameters[5] * (*it_vi - parameters[6])
                );
    }
}
std::vector<double> HH_model_class::return_result_current_L() const
{
    return result_current_L;
}
