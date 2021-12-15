#include "BlackScholes.h"
#include <math.h>
#include <iostream>
#include <random>

double BlackScholes::N_estimation(double z) {

	if (z > 6.0)  // Normal prob estimation using Taylor series
		return 1.0;  //avoid non valid values
	if (z < -6.0)
		return 0.0;
	double b1 = 0.31938153;
	double b2 = -0.356563782;
	double b3 = 1.781477937;
	double b4 = -1.821255978; 
	double b5 = 1.330274429; 
	double p = 0.2316419; 
	double c2 = 0.3989423; 
	double a = fabs(z); 
	double t = 1.0 / (1.0 + a * p); 
	double b = c2 * exp((-z)*(z / 2.0)); 
	double n = ((((b5*t + b4)*t + b3)*t + b2)*t + b1)*t; n = 1.0 - b * n; 
	if (z < 0.0)
		n = 1.0 - n; 
	return n;
}

double BlackScholes::get_d1() {

	double d1;
	d1 = (1 / (sigma * sqrt(option->T))) * (log(option->s0 / double(option->K)) + (r + 0.5 * pow(sigma, 2)) * option->T);
	return d1;
}


double BlackScholes::get_price(std::string res) {

	double price = 0.0;

	if (res == "CALL") // just using a string argument to determine if we want to compute a CALL formula or a PUT
		price = option->s0 * N_estimation(get_d1()) - option->K * exp(-r * option->T) * N_estimation(get_d1() - sigma * sqrt(option->T));
	else
		price = -option->s0 * N_estimation(-get_d1()) + option->K * exp(-r * option->T) * N_estimation(-(get_d1()- sigma * sqrt(option->T)));

	return price;
}

double BlackScholes::get_delta(std::string res) {

	double delta;

	if (res == "CALL") {
		delta = exp(-q * option->T) * N_estimation(get_d1());
	}
	else
		delta = exp(-q * option->T) * N_estimation(get_d1()) - 1;

	return delta;
}
