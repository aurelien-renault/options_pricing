
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include "RandomWalk.h"
#include "BlackScholes.h"
#include "CRR.h"
#include "Generator.h"
#include "Monte_Carlo.h"
#include "Option.h"

using namespace std;

void random_Walk() {

	int step; int s0; double prob = 0; 

	cout << "Number of step ? ";
	cin >> step;
	cout << "Starting value ? ";
	cin >> s0;
	//cout << "Probability up ? ";
	//cin >> prob;

	Generator::Exponential;

	vector<double> tab;
	tab = Generator::generate(step, s0, 0, 1, 0, 1);

	//for (int i = 0; i < step; i++) {
	//	std::cout << tab[i] << endl;
	//}

	RandomWalk rd;
	rd.set(step, s0, prob);
	rd.store_values(tab);

	cout << "Average is : " << rd.average() << endl;
	cout << "Variance is : " << rd.variance() << endl;

	rd.print_values();

	cout << "Second element is : " << rd[1] << endl;
}

void black_Scholes() {

	int strike; double date; int sous_jacent; double interest; double vol; double dividend; string tmp;
	Option *opt;
	
	cout << "Strike price : ";
	cin >> strike;
	cout << "Underlaying value : ";
	cin >> sous_jacent;
	cout << "Time before expiracy (in years) : ";
	cin >> date;



	cout << "Interest rate : ";
	cin >> interest;
	cout << "Dividend yield : ";
	cin >> dividend;
	cout << "Volatility of the price : ";
	cin >> vol;
	
	cout << "Option type (CALL / PUT) : ";
	cin >> tmp;

	if (tmp == "CALL") {
		European_Call euro;
		euro.set(sous_jacent, strike, date);
		opt = &euro;
	}
	else {
		European_Put euro;
		euro.set(sous_jacent, strike, date);
		opt = &euro;
	}

	BlackScholes BS;
	BS.set(interest, dividend, vol, opt);

	cout << "Price is " << BS.get_price(tmp) << " dollars" << endl;
	//cout << BS.get_call_price() << endl;
	//cout << BS.get_put_price();

	cout << "Delta is " << BS.get_delta(tmp) << endl;

}

void binomial_tree() {

	int strike; double date; int step; int sous_jacent; double interest; double vol; string tmp; string method;
	Option *opt;

	cout << "Strike price : ";
	cin >> strike;
	cout << "Underlaying value : ";
	cin >> sous_jacent;
	cout << "Time before expiracy (in years) : ";
	cin >> date;


	cout << "Interest rate : ";
	cin >> interest;
	cout << "Volatility of the price : ";
	cin >> vol;
	cout << "Number of step in the tree : ";
	cin >> step;


	cout << "Option type (CALL / PUT) : ";
	cin >> tmp;

	if (tmp == "CALL") {
		European_D_Call euro;
		euro.set(sous_jacent, strike, date);
		opt = &euro;
	}	
	else {
		European_Put euro;
		euro.set(sous_jacent, strike, date);
		opt = &euro;
	}

	CRR tree;
	tree.set(step, interest, vol, opt);

	cout << "Initialize tree with Black-Scholes parameters ? (YES/NO)" << endl;
	cin >> tmp;
	tree.set_UpDown(tmp, 1000);

	double *pt;
	pt = tree.create_last_prices();

	tree.store_prices(pt, step);
	tree.display();

	
	cout << "At expiry date, option prices are : " << endl;
	for (int i = 0; i <= step; i++) {
		cout << pt[i] << endl;
	}

	//tree.american(pt, step);
	//tree.get_prices_american(0);
	//tree.get_prices_american(1);
	//tree.get_prices_american(2);

	//tree.store_prices(pt, step);
	//tree.get_prices(4);

	cout << "What method do you want to use ? (ITERATIVE / CLOSED FORM (time == nb of step)) " << endl;
	cin >> method;

	if (method == "ITERATIVE")
		cout << "Option price using iterative method is " << tree.get_price(pt, step) << " dollars" << endl;
	else
		cout << "Option price using closed-form formula is " << tree.get_price_closed_form(pt) << " dollars" << endl;

	
}

void monte_carlo() {

	int strike; double date; int step; int sous_jacent; double interest; double vol; int draw; string tmp; string method;
	Option *opt;

	cout << "Strike price : ";
	cin >> strike;
	cout << "Underlaying value : ";
	cin >> sous_jacent;
	cout << "Time before expiracy (in years) : ";
	cin >> date;


	cout << "Interest rate : ";
	cin >> interest;
	cout << "Volatility of the price (sigma): ";
	cin >> vol;

	cout << "Number of draws for Monte Carlo estimation : ";
	cin >> draw;
	cout << "Number of periods (m) : ";
	cin >> step;

	cout << "Option type (CALL / PUT) : ";
	cin >> tmp;

	if (tmp == "CALL") {
		//European_Call euro;
		//euro.set(sous_jacent, strike, date);
		//opt = &euro;

		Asian_Call asia;
		asia.set(sous_jacent, strike, date);
		opt = &asia;
	}
	else {
		//European_Put euro;
		//euro.set(sous_jacent, strike, date);
		//opt = &euro;

		Asian_Put asia;
		asia.set(sous_jacent, strike, date);
		opt = &asia;
	}

	
	Monte_Carlo mc;
	mc.set(step, draw, interest, vol, opt);

	string res = "non"; int n;
	do
	{
		//cout << "Estimation for the euro option using Monte Carlo method is : " << mc.estimation_euro() << " dollars" << endl;
		cout << "Estimation for the asian option using Monte Carlo method is : " << mc.estimation_asian() << " dollars" << endl;
		cout << "Voulez-vous retirer un nb de points (oui/non) ? " << endl;
		cin >> res;
		if (res != "oui")
			break;
		cout << "Combien de points supplementaires ?";
		cin >> n;
		mc.N = n;

	} while (res == "oui");

	//for (int i = 0; i < step * draw ; i++) {
   //		cout << mc.prices[i] << endl;
	//}
}

int main()
{
	//random_Walk();
	black_Scholes();
	//binomial_tree();
	//monte_carlo();
}

