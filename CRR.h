#pragma once
#include "Option.h"
#include <vector>

class CRR
{
public:

	int n_step;
	double R;
	double sigma;
	double Up;
	double Down;
	std::vector<std::vector<double>> prices;
	std::vector<std::vector<bool>> optimality;
	Option* option;

public:

	void set(int n_, double R_, double sigma_, Option *opt) {
		n_step = n_; R = R_; sigma = sigma_; option = opt;
	}
	void set_UpDown(std::string rep) {
		Up = exp(sigma * pow((option->T / float(n_step)), 0.5)) - 1;
		Down = 1 / (Up + 1) - 1;

		if (rep == "YES" || rep == "yes") {
			// already implement a method that could subdivide time interval into n_step, we just re-use n_step here 
			// changing a little bit the UP/DOWN formulas
			Up = exp((R - pow(sigma, 2) / 2) * option->T / float(n_step) + sigma * pow(option->T / float(n_step), 0.5)) - 1;
			Down = exp((R - pow(sigma, 2) / 2) * option->T / float(n_step) - sigma * pow(option->T / float(n_step), 0.5)) - 1;
			//didn't use the same formulas, to compute prices in the tree, don't need R formula
			// change it by exp(-r * T/n_step) - 1
		}
	}

	double *create_last_prices();

	double get_price(double tab[], int n);

	double get_price_closed_form(double tab[]);
	int factoriel(int val);

	void store_prices(double tab[], int n);
	void get_prices(int step);

	void american(double tab[], int n);
	void get_prices_american(int step);

	void display();
};

