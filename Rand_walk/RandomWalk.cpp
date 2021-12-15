#include "RandomWalk.h"

using namespace std;

double RandomWalk::operator[](int t) {

	if (t < 0 || t > n) {
		throw "Invalid index";
	}
	return values[t];
}

double RandomWalk::average() {
	double sum = 0;
	for (int i = 0; i < n; i++) {
		sum += values[i];
	}
	return sum / n;
}

double RandomWalk::variance() {
	double sqDiff = 0;
	double mean = average();
	for (int i = 0; i < n; i++) {
		sqDiff += (values[i] - mean) * (values[i] - mean);
	}		
	return sqDiff / n;
}

void RandomWalk::print_values() {

	for (int i = 0; i < n; i++) {
		std::cout << values[i] << endl;
	}
}

void RandomWalk::store_values(std::vector<double> tab) {

	for (int i = 0; i < n; i++) {
		values.push_back(tab[i]);
	}
}
