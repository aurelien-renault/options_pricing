#pragma once
#include <vector>

class Option
{
public:

	int K;
	int s0;
	double T;

public:
	void set(int s0_, int K_, double T_) {
		s0 = s0_; K = K_; T = T_; 
	}
	virtual double payoff(double z) = 0; 
	virtual double payoff(std::vector<double> z, int m) = 0;// had to do 2	pure virtual method, with one for asian options payoffs

};


class European_Call : public Option
{
public:

	double payoff(double z) {

		double payoff = 0.0;
		if (z > K)
			payoff = z - K;

		return payoff;
	}

	double payoff(std::vector<double> z, int m) { return 0; }
};

class European_Put : public Option
{
public:

	double payoff(double z) {

		double payoff = 0.0;
		if (z < K)
			payoff = K - z;

		return payoff;
	}

	double payoff(std::vector<double> z, int m) { return 0; }
};

class European_D_Call : public Option
{
	double payoff(double z) {

		double payoff = 0.0;
		if (z > K)
			payoff = 1.0;

		return payoff;
	}

	double payoff(std::vector<double> z, int m) { return 0; }
};

class European_D_Put : public Option
{
	double payoff(double z) {

		double payoff = 0.0;
		if (z < K)
			payoff = 1.0;

		return payoff;
	}

	double payoff(std::vector<double> z, int m) { return 0; }
};

class Asian_Call : public Option
{
	double payoff(std::vector<double> z, int m) {
		double payoff = 0.0; double sum = 0.0;
		for (int i = 0; i < m; i++)
			sum += z[i] / m;

		if (sum > K)
			payoff = sum - K;

		return payoff;
	}

	double payoff(double z) { return 0; }
};

class Asian_Put : public Option
{
	double payoff(std::vector<double> z, int m) {
		double payoff = 0.0; double sum = 0.0;
		for (int i = 0; i < m; i++)
			sum += z[i] / m;

		if (sum < K)
			payoff = K - sum;

		return payoff;
	}
	double payoff(double z) { return 0; }
};
