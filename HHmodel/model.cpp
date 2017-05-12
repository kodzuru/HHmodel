
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <list>
using namespace std;


//define functions --------------------------
double i_Na();
double i_K();
double i_L();

double val_n(double ni);
double val_m(double mi);
double val_h(double hi);

double val_V(double Vi);


//begin variables ------------------------------
//time
double ti, t0, tn;

//iteration
int i;
int n;

double s;

//potential
double V0, Vi;

//membrane permeability
double n0, ni;
double m0, mi;
double h0, hi;

//---permeability

double alpha_n;
double beta_n;

double alpha_m;
double beta_m;

double alpha_h;
double beta_h;

//------constants
double C;

double VNa;
double VK;
double VL;

double gNa;
double gK;
double gL;


int main1() {



	//begin initialising variables ----------------------

	t0 = 0;               //Intervall - START
	tn = 10;            //Intervall - STOP
	n = 500;            //SAMPLES
	i = 0;                //Sample-count

	s = (tn - t0) / n;    //step range (increment)


	//constants declaration

	C = 1;          //Capacity in uF*cm^-2

	VNa = 50;       //Sodium equilibrium potential in mV 50
	VK = -77;      //Potassium equilibrium potential in mV -77
	VL = -54.387;  //Leak potential in mV -54.387


	gNa = 120;      //Sodium conductance in mS * cm^-2
	gK = 36;        //Potassium conductance in mS * cm^-2
	gL = 0.3;       //Leakage 'conductance' in mS * cm^-2


	//initialising starting variables

	V0 = 0;          //0

	n0 = 0.31768;  //0.31768
	h0 = 0.59612;    //0.59612
	m0 = 0.052932;   //0.052932

	Vi = V0;

	ni = n0;
	mi = m0;
	hi = h0;


	//end initialising variables --------------------


	list<double> vec_vi;
	list<double> vec_ti;

	ofstream daten;
	daten.open("werte.txt");


	for (i; i <= n; i++)
	{

		ti = t0 + s * i;


		//---data-log

		vec_vi.push_back(Vi);
		vec_ti.push_back(ti);

		//daten << ti;
		//daten << "\t";
		//daten << Vi;
		//daten << "\n";


		//mi ------------

		mi = val_m(mi);

		//ni ------------

		ni = val_n(ni);

		//hi ------------

		hi = val_h(hi);



		//constrain m,n,h ------------
		if (mi <= 0.000001) {
			mi = 0.000001;
		}
		if (mi >= 1) {
			mi = 1;
		}
		if (ni <= 0.000001) {
			ni = 0.000001;
		}
		if (ni >= 1) {
			ni = 1;
		}
		if (hi <= 0.000001) {
			hi = 0.000001;
		}
		if (hi >= 1) {
			hi = 1;
		}


		//Membrane Potential --------------


		Vi = val_V(Vi);
		//--------------------------
	}
	//daten.close();

	list<double>::iterator result = min_element(vec_vi.begin(), vec_vi.end());
	for (std::list<double>::iterator it = vec_vi.begin(); it != vec_vi.end(); ++it) {
		daten << *it << "\n";
	}
	daten.close();

	//cout << *result << endl;
	//cout << distance(vec_vi.begin(), result) << endl;
	cout << "done!";
	_getch();
	return 0;

}

//begin formulae -----------------------

//individual currents ******************

//i_Na sodium current
double i_Na()
{
	return (gNa  * pow(mi, 3) * hi * (Vi - VNa));
}

//i_K potassium current
double i_K()
{
	return (gK * pow(ni, 4) * (Vi - VK));
}

//i_L leakage current
double i_L()
{
	return (gL * (Vi - VL));
}


//conductance activation and inhibition ****************************
//potassium channel activation factor
double val_n(double ni)
{
	alpha_n = (0.01 * (Vi + 10)) / exp(((Vi + 10) / 10) - 1);
	beta_n = 0.125 * exp(Vi / 80);

	return (ni + s * (alpha_n * (1 - ni) - beta_n * ni));
}

//sodium channel activation factor
double val_m(double mi)
{
	alpha_m = (0.1 * (Vi + 25)) / exp(((Vi + 25) / 10) - 1);
	beta_m = 4 * exp(Vi / 18);

	return (mi + s * (alpha_m * (1 - mi) - beta_m * mi));
}

//sodium channel inhibition factor
double val_h(double hi)
{
	alpha_h = 0.07 * exp(Vi / 20);
	beta_h = 1 / (exp((Vi + 30) / 10) + 1);

	return (hi + s * (alpha_h * (1 - hi) - beta_h * hi));
}



//membrane potential **************************
double val_V(double Vi)
{
	return (Vi + s * ((0 - (i_Na() + i_K() + i_L())) / C));
}

