#pragma once
#include <vector>
#include <random>
#include "Option.h"

class Monte_Carlo
{
public:

	double r;
	double sigma;
	int m;
	int N, Nprev;
	double est_price = 0; // attribute to store the current estimation of option price
	std::vector<double> prices;
	std::vector<double> estimations; // store each sample estimation for confidence interval
	Option *option;

public:

	void set(int m_, int N_, double r_, double sigma_, Option *opt) {
		m = m_;  N = N_; r = r_; sigma = sigma_; Nprev = 0; option = opt;
	}

	void calculate_prices(std::mt19937 gen, int index); // fill prices vector
	double estimation_asian();
	double estimation_euro();
	std::vector<double> sample_path(); // optional method to create random values following normal law, didn't use in the program
};

