#include "Generator.h"
#include <random>
#include <math.h>


std::vector<double> Generator::generate(int n, double S0, double mean = 0, double sigma = 1, double p = 0.5, double lambda = 1) {

	//Generator function just taking in argument every possible parameters for our different distributions

	std::random_device rd;
	std::mt19937 gen(rd());

	std::vector<double> tab(n); // vector in which  we will store our randomWalk values
	tab[0] = S0; // initializing first element

	if (Generator::Bernoulli) { //testing enum types 

		std::uniform_real_distribution<> dis(0.0, 1.0);

		double a;
		double sigma = float(p) * (1 - p);

		for (int i = 1; i < n; i++) {

			a = dis(gen);

			if (a < p) {
				tab[i] = tab[i - 1] * exp(sigma);
			}
			else
				tab[i] = tab[i - 1] * exp(-sigma);
		}
	}
	else
		if (Generator::Normale) {

			std::normal_distribution<> dis(mean, sigma);

			for (int i = 1; i < n; i++) {
				tab[i] = tab[i - 1] + dis(gen);
			}
		}
		else
			if (Generator::Exponential) {

				std::exponential_distribution<> dis(lambda);

				for (int i = 1; i < n; i++) {
					tab[i] = tab[i - 1] + dis(gen);
				}
			}

	std::ofstream outFile("values.csv"); // print our values in a .csv file
	for (const auto &e : tab) outFile << e << "\n";
	std::cout << "new .csv file created names 'values' in the Project folder" << std::endl; // to use our data in excel, have to replace '.' by ','

	return tab;
	
}
