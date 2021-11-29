#include "CRR.h"
#include <math.h>
#include <iostream>
#include <iomanip> 


double* CRR::create_last_prices() {

	double *pt;
	pt = new double[n_step];

	double underlying;
	std::cout << "up is : " << Up << std::endl;
	std::cout << "down is : " << Down << std::endl;

	for (int i = 0; i <= n_step; i++) {
		underlying = option->s0 * pow((1 + Up), i) * pow((1 + Down), n_step - i);
		pt[i] =  option->payoff(underlying);
	}

	return pt;
}

double CRR::get_price(double tab[], int n) {

	double p = (exp(R * option->T / float(n_step)) - (Down + 1)) / ((Up + 1) - (Down + 1));
	// that's not the formula provided in TDs, but that we found in optional ressources on DVO 

	if (n == -1) { //end condition of recursive algo
		return tab[0]; // finally the option price at t=0 is the first element of tab 
	}
	else {
		for (int i = 0; i <= n - 1; i++) {
			tab[i] = exp(-R * option->T / float(n_step)) * (p * tab[i + 1] + (1 - p) * tab[i]);
		}
		return get_price(tab, n - 1); // crash the tab values taken in argument each time the function call itself
	}
}

double CRR::get_price_closed_form(double tab[]) {

	double p = (exp(R * option->T / float(n_step)) - (Down + 1)) / ((Up + 1) - (Down + 1));
	//double q = (R - Down) / (Up - Down);

	double price = 0;

	// just worked for maturity == time steps in our tree, don't see how to adapt the method if these values are different
	for (int i = 0; i <= n_step; i++) {
		price += (1 / pow((1 + R), n_step)) * (factoriel(n_step) / (factoriel(i) * factoriel((n_step - i)))) * pow(p, i) * pow(1 - p, n_step - i) * tab[i];
	}

	return price;
}

int CRR::factoriel(int val)
{
	if (val == 0)
		return 1;
	else
		return val * factoriel(val - 1);
}


void CRR::store_prices(double tab[], int n) {

	double p = (exp(R * option->T / float(n_step)) - (Down + 1)) / ((Up + 1) - (Down + 1));

	std::vector<double> tmp;
	for (int i = 0; i <= n; i++) {
		tmp.push_back(tab[i]);
	}
	prices.push_back(tmp); // fill our first element with the vector of last prices 

	int index = 0;

	for (int j = 0; j <= n_step; j++) {

		std::vector<double> tmp1;

		for (int k = 0; k <= n - 1; k++) {
			tmp1.push_back(exp(-R * option->T / float(n_step)) * (p * prices[index][k + 1] + (1 - p) * prices[index][k]));
		}
		prices.push_back(tmp1);
		n--; index++;
	}
}

void CRR::get_prices(int step) {

	for (int i = 0; i <= step; i++) {
		std::cout << prices[n_step - step][i] << std::endl;
	}
	
}


void CRR::american(double tab[], int n) {

	double p = (exp(R * option->T / float(n_step)) - (Down + 1)) / ((Up + 1) - (Down + 1));

	std::vector<double> tmp;
	for (int i = 0; i <= n; i++) {
		tmp.push_back(tab[i]);
	}
	prices.push_back(tmp);

	int index = 0;

	for (int j = 0; j <= n_step; j++) {
		std::vector<double> tmp1;
		std::vector<bool> opti; // vector of vectors of bool to store whenever it's optimal or not to exercise the option

		double cont; double exer;
		for (int k = 0; k <= n - 1; k++) {
			cont = exp(-R * option->T / float(n_step)) * (p * prices[index][k + 1] + (1 - p) * prices[index][k]);
			exer = option->payoff(option->s0 * pow((1 + Up), k) * pow((1 + Down), n - 1 - k));

			if (exer > cont) {
				tmp1.push_back(exer);
				opti.push_back(true);
			}
			else {
				tmp1.push_back(cont);
				opti.push_back(false);
			}
		}

		prices.push_back(tmp1);
		optimality.push_back(opti);
		n--; index++;
	}
}

void CRR::get_prices_american(int step) {

	for (int i = 0; i <= step; i++) {
		std::cout << prices[n_step - step][i] << std::endl;
		if (optimality[n_step - step - 1][i] == true) {
			std::cout << "It's optimal to exercise at node " << i << std::endl;
		}
		else
			std::cout << "It's not optimal to exercise at node " << i << std::endl;
	}
	std::cout << " " << std::endl;
}

void CRR::display() {
	int espace = 50; // first element of the tree is displayed in the middle of the console 
	std::cout.setf(std::ios::fixed);
	std::cout.setf(std::ios::showpoint);
	std::cout.precision(2); // every prices should be displayed with 2 numbers after the coma
	for (int j = 0; j <= n_step; j++) {
		
		for (int i = 0; i <= j; i++) {
			if (i == 0)
				std::cout << std::setw(espace);
			std::cout << prices[n_step - j][i] << "  ";
		}
		std::cout << std::endl;

		for (int i = 0; i <= j; i++) {
			if (i == 0)
				std::cout << std::setw(espace-2);
			if (j == n_step)
				break;
			std::cout << "/ " << " \\  ";
		}
		std::cout << std::endl;
		espace -= 3;
	}
}