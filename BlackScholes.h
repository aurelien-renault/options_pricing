#pragma once
#include <string>
#include <math.h>
#include "Option.h"

class BlackScholes
{
public:

	double r;
	double q;
	double sigma;
	Option *option;

public:
	void set(double r_, double q_, double sigma_, Option *opt) {
		r = r_; q = q_; sigma = sigma_; option = opt;
	}

	double N_estimation(double z);
	double get_d1();
	double get_price(std::string res);
	double get_delta(std::string res);
};

