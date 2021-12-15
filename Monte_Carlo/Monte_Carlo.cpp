#include "Monte_Carlo.h"
#include <vector>
#include <random>
#include <math.h>
#include <iostream>

void Monte_Carlo::calculate_prices(std::mt19937 gen, int index) {

	std::normal_distribution<> dis(0.0, 1.0); //Normale law of param (0,1)
	std::vector<double> rand_values;
	rand_values = sample_path();

	double s1;
	s1 = option->s0 * exp((r - pow(sigma, 2) * 0.5) * option->T / m + sigma * sqrt(option->T / m) * dis(gen));

	prices.push_back(s1); // first element of our vector

	for (int i = index * m; i < (index + 1) * m - 1; i++) {
		double sk; 
		sk = prices[i] * exp((r - pow(sigma, 2) * 0.5) * option->T / m + sigma * sqrt(option->T/m) * dis(gen)); // tk - tk-1 = T/m bcse tk = kT/

		prices.push_back(sk);
	}
	//rand_values.clear();
}

double Monte_Carlo::estimation_asian() {
	
	double new_price = 0.0; double price_tmp = 0.0;
	
	for (int n = 0; n < N; n++) {

		std::random_device rd;
		std::mt19937 gen(rd());

		calculate_prices(gen, n);

		std::vector<double> tmp; // new vector to store values from 1 random path
		for (int j = n * m; j < (n + 1) * m; j++) {
			tmp.push_back(prices[j]); // fill the new vector 
		}
		
		price_tmp += exp(-r * option->T) / N * option->payoff(tmp,m);
		estimations.push_back(exp(-r * option->T) * option->payoff(tmp, m));
		tmp.clear(); // we reboot the temporary vector, getting ready for the next simulation
		
	}

	if (est_price != 0) // if this is not the first estimation
	{
		new_price = ((est_price * Nprev) + (price_tmp * N)) / (Nprev + N);
		est_price = new_price;
	}
	else
		est_price = price_tmp;

	Nprev += N; // update number of simulation

	// compute interval of confidence by computing variance 
	double diff2 = 0.0; double var = 0.0;
	for (int i = 0; i < Nprev; i++) {
		diff2 += pow(est_price - estimations[i], 2);
	}
	var = 1 / float(Nprev - 1) * diff2;
	std::cout << var << std::endl;
	std::cout << "95% interval of confidence is [" << est_price - 1.96*pow(var / Nprev, 0.5) << " ; " << est_price + 1.96*pow(var / Nprev, 0.5) << "]" << std::endl;

	return est_price;
}

double Monte_Carlo::estimation_euro() {
	
	double new_price = 0.0; double price_tmp = 0.0;

	for (int n = 0; n < N; n++) { // looping on the initial number of simulations desired

		std::random_device rd;
		std::mt19937 gen(rd());
		calculate_prices(gen, n);

		price_tmp += exp(-r * option->T) * option->payoff(prices[(n+1) * m - 1]) / N;
		estimations.push_back(exp(-r * option->T) * option->payoff(prices[(n + 1) * m - 1]));
	}

	//for (int j = 0; j < N; j = j+2) {
		//price_tmp += exp(-r * option->T) * 2 * option->payoff((prices[(j + 1) * m - 1] + prices[(j + 2) * m - 1]) / 2) / N;
		//estimations.push_back(exp(-r * option->T) * option->payoff((prices[(j + 1) * m - 1] + prices[(j + 2) * m - 1]) / 2));
	//}

	if (est_price != 0) // if this is not the first estimation we run, re-calcule the new estimation
	{
		new_price = (((est_price * Nprev) + (price_tmp * N)) / (Nprev + N));
		est_price = new_price;
	}
	else
		est_price = price_tmp; // first estimation

	Nprev += N; // Number of simulations update

	// compute interval of confidence by computing variance 
	double diff2 = 0.0; double var = 0.0;
	for (int i = 0; i < Nprev; i++) {
		diff2 += pow(est_price - estimations[i], 2);
	}
	var = 1 / float(Nprev - 1) * diff2;
	std::cout << var << std::endl;
	std::cout << "95% interval of confidence is [" << est_price - 1.96*pow(var / Nprev, 0.5) << " ; " << est_price + 1.96*pow(var / Nprev, 0.5) << "]" << std::endl;
	std::cout << "99% interval of confidence is [" << est_price - 2.576*pow(var / Nprev, 0.5) << " ; " << est_price + 2.576*pow(var / Nprev, 0.5) << "]" << std::endl;

	return est_price;
}

std::vector<double> Monte_Carlo::sample_path() { 

	static std::random_device rd; //alternative way to generate random path
	std::mt19937 gen(rd());
	static std::normal_distribution<> dis(0.0, 1.0);

	std::vector<double> path; double sum = 0.0;

	for (int i = 0; i < m; i++) {
		path.push_back(dis(gen));
	}
	
	return path;
} 
