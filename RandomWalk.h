#pragma once
#include <iostream>
#include <vector>
#include "Generator.h"

class RandomWalk
{
public:
	int n;
	int s0;
	std::vector<double> values;
	double p;
	double mu;
	double sigma;
	double lambda;
	Generator::distribution dist;

public:
	//void set_dist(std::string res) { }
	int get_nbsteps() {return n;}

	void set(int n_, int s0_) {
		n = n_; s0 = s0_;  
	}
	double average();
	double variance();
	double operator[](int t);
	void print_values();
	void store_values(std::vector<double> tab);
};
