#include "linear_alg.h"
#include "RKmethod.h"
#include "CSVReader.h"

int main()
{
	Matrix<double> state(2,1);
	state[0][0] = -1.;
	state[1][0] = 0.;
	RKintegrator<RKmethod_5_4_7fs, Osc_Van_der_Pol> integ1;
//	RKintegrator<VernerRK_5_6_8, Osc_Van_der_Pol> integ2;
	std::vector<Matrix<double>> res1 = integ1.n_steps(0, state, 0.01, 50);
	for (auto i : res1)
	{
		cout << i << endl << "-----" << endl;
	}
	//std::vector<Matrix<double>> res2 = integ2.n_steps(0, state, 0.005, 50000);
	//write_in_file("c:\\Users/egorz/Desktop/osc_1.csv", res1);
	//write_in_file("c:\\Users/egorz/Desktop/osc_2.csv", res2);
	system("pause");
	return 0;
}