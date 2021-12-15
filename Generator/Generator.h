#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class Generator 
{
public:

	enum distribution {Bernoulli, Normale, Exponential};
	distribution d;

	Generator& operator=(const Generator&) = delete;
	Generator(const Generator&) = delete;
	Generator() {}
	~Generator() {}

public:
	static std::vector<double> generate(int n, double S0, double mean, double sigma, double p, double lambda);
};

