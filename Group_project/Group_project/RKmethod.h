#pragma once
#include "linear_alg.h"	
#include "Threads.h"
#include "CSVReader.h"
#include <sstream>
#include <typeinfo>

class Ode
{
public:
	virtual Matrix<double> operator()(double, Matrix<double>) = 0;
	virtual void set_const(vector<double>) = 0;
};

class LVOde: public Ode
{
public:
	Matrix<double> operator()(double t, Matrix<double>state_vector) //returns ds/dt for s and t
	{
		Matrix<double> tmp(2, 1);
		tmp[0][0] = state_vector[0][0] * (a - b * state_vector[1][0]);
		tmp[1][0] = state_vector[1][0] * (-c + d * state_vector[0][0]);
		return tmp;
	}
	void set_const(vector<double> consts)
	{
		if (consts.size() != 4)
		{
			throw out_of_range("Doesn't match with consts number");
		}
		a = consts[0];
		b = consts[1];
		c = consts[2];
		d = consts[3];
	}
private:
	double a;
	double b;
	double c;
	double d;
};

class Osc_Van_der_Pol: public Ode
{
public:
	Matrix<double> operator()(double t, Matrix<double>state_vector) //returns ds/dt for s and t
	{
		Matrix<double> tmp(2, 1);
		double x = state_vector[0][0];
		double y = state_vector[1][0];
		tmp[0][0] = mu * (x - (x*x*x) / 3 - y);
		tmp[1][0] = x / mu;
		return tmp;
	}
	void set_const(vector<double> consts)
	{
		if (consts.size() != 1)
		{
			throw out_of_range("Doesn't match with consts number");
		}
		mu = consts[0];
	}
private:
	double mu;
};

class Attractor_Lorence: public Ode
{
public:
	Matrix<double> operator()(double t, Matrix<double>state_vector) //returns ds/dt for s and t
	{
		Matrix<double> tmp(3, 1);
		double x = state_vector[0][0];
		double y = state_vector[1][0];
		double z = state_vector[2][0];
		tmp[0][0] = sigma * (y - x);
		tmp[1][0] = x * (r - z) - y;
		tmp[2][0] = x * y - b * z;
		return tmp;
	}
	void set_const(vector<double> consts)
	{
		if (consts.size() != 3)
		{
			throw out_of_range("Doesn't match with consts number");
		}
		sigma = consts[0];
		r = consts[1];
		b = consts[2];
	}
private:
	double sigma;
	double r;
	double b;
};

class Chua_chain: public Ode
{
public:
	Matrix<double> operator()(double t, Matrix<double>state_vector) //returns ds/dt for s and t
	{
		Matrix<double> tmp(3, 1);
		double x = state_vector[0][0];
		double y = state_vector[1][0];
		double z = state_vector[2][0];
		double h = m1 * x + 1. / 2 * (m0 - m1)*(fabs(x + 1) - fabs(x - 1));
		tmp[0][0] = alpha * (y - x - h);
		tmp[1][0] = x - y + z;
		tmp[2][0] = -beta * y;
		return tmp;
	}
	void set_const(vector<double> consts)
	{
		if (consts.size() != 4)
		{
			throw out_of_range("Doesn't match with consts number");
		}
		alpha = consts[0];
		beta = consts[1];
		m1 = consts[2];
		m0 = consts[3];
	}
private:
	double alpha = 7.;
	double beta = 10.;
	double m1 = -5. / 7;
	double m0 = -8. / 7;
};

class RKmethod_5_4_7fs // (5,6)
{
public:
	RKmethod_5_4_7fs()
	{
		A = Matrix<double>(7, 7);
		for (size_t i = 0; i < 7; i++)
			A[0][i] = 0.;

		A[1][0] = 2. / 9;
		for (size_t i = 1; i < 7; i++)
			A[1][i] = 0.;

		A[2][0] = 1. / 12;
		A[2][1] = 1. / 4;
		for (size_t i = 2; i < 7; i++)
			A[2][i] = 0.;

		A[3][0] = 55. / 325;
		A[3][1] = -25. / 108;
		A[3][2] = 50. / 81;
		for (size_t i = 3; i < 7; i++)
			A[3][i] = 0.;

		A[4][0] = 83. / 330;
		A[4][1] = -13. / 22;
		A[4][2] = 61. / 66;
		A[4][3] = 9. / 100;
		for (size_t i = 4; i < 7; i++)
			A[4][i] = 0.;

		A[5][0] = -19. / 28;
		A[5][1] = 9. / 4;
		A[5][2] = 1. / 7;
		A[5][3] = -27. / 7;
		A[5][4] = 22. / 7;
		A[5][5] = 0.;
		A[5][6] = 0.;

		A[6][0] = 19. / 200;
		A[6][1] = 0.;
		A[6][2] = 3. / 5;
		A[6][3] = -243. / 400;
		A[6][4] = 33. / 40;
		A[6][5] = 7. / 80;
		A[6][6] = 0.;

		c = Matrix<double>(7, 1);
		c[0][0] = 0.;
		c[1][0] = 2. / 9;
		c[2][0] = 1. / 3;
		c[3][0] = 5. / 9;
		c[4][0] = 2. / 3;
		c[5][0] = 1.;
		c[6][0] = 1.;

		b_main = Matrix<double>(7, 1);
		b_main[0][0] = 19. / 200;
		b_main[1][0] = 0.;
		b_main[2][0] = 3. / 5;
		b_main[3][0] = -243. / 400;
		b_main[4][0] = 33. / 40;
		b_main[5][0] = 7. / 80;
		b_main[6][0] = 0.;

		b_subs = Matrix<double>(7, 1);
		b_subs[0][0] = 431. / 5000;
		b_subs[1][0] = 0.;
		b_subs[2][0] = 333. / 500;
		b_subs[3][0] = -7857. / 10000;
		b_subs[4][0] = 957. / 1000;
		b_subs[5][0] = 193. / 2000;
		b_subs[6][0] = -1. / 50;

		/*
				orders=[5;4];
				isFSAL=true;
		*/
	}
	Matrix<double> execute(double t_0, Matrix<double>& y_0, double h, Ode* f)
	{
		std::vector<Matrix<double>> k;
		Matrix<double> tmp_y(y_0.get_amount_of_lines(), 1);
		k.push_back((*f)(t_0, y_0));
		for (size_t i = 1; i < 7; i++)
		{
			tmp_y = tmp_y * 0;
			for (size_t j = 0; j < i; j++)
			{
				tmp_y = tmp_y + A[i][j] * k[j];
			}
			k.push_back((*f)(t_0 + c[i][0] * h, y_0 + tmp_y * h));
		}
		Matrix<double> tmp_acc(y_0.get_amount_of_lines(), 1, 0.);
		tmp_y = y_0;
		tmp_acc = y_0;
		for (size_t i = 0; i < 7; i++)
		{
			tmp_y = tmp_y + h * b_main[i][0] * k[i];
			tmp_acc = tmp_acc + h * b_subs[i][0] * k[i];
		}

		Matrix<double> ttmp_y(tmp_y.get_amount_of_lines() + 1, 1);
		for (size_t i = 0; i < tmp_y.get_amount_of_lines(); i++)
			ttmp_y[i][0] = tmp_y[i][0];
		ttmp_y[tmp_y.get_amount_of_lines()][0] = (tmp_acc - tmp_y).mod();
		return ttmp_y;
	}
private:
	Matrix<double> A;
	Matrix<double> c;
	Matrix<double> b_main;
	Matrix<double> b_subs;
};

class VernerRK_5_6_8 // (5,8)
{
public:
	VernerRK_5_6_8()
	{
		A = Matrix<double>(8, 8, 0.);
		A[1][0] = 1. / 18;
		A[2][0] = -1. / 12;
		A[2][1] = 1. / 4;
		A[3][0] = -2. / 81;
		A[3][1] = 4. / 27;
		A[3][2] = 8. / 81;
		A[4][0] = 40. / 33;
		A[4][1] = -4. / 11;
		A[4][2] = -56. / 11;
		A[4][3] = 54 / 11;
		A[5][0] = -369. / 73;
		A[5][1] = 72. / 73;
		A[5][2] = 5380. / 219;
		A[5][3] = -12285. / 584;
		A[5][4] = 2695. / 1752;
		A[6][0] = -8716. / 891;
		A[6][1] = 656. / 297;
		A[6][2] = 39520. / 891;
		A[6][3] = -416. / 11;
		A[6][4] = 52. / 27;
		A[7][0] = 3015. / 256;
		A[7][1] = -9. / 4;
		A[7][2] = -4219. / 78;
		A[7][3] = 5985. / 128;
		A[7][4] = -539. / 384;
		A[7][5] = 0.;
		A[7][6] = 693. / 3328;

		c = Matrix<double>(8, 1);
		c[0][0] = 0.;
		c[1][0] = 1. / 18;
		c[2][0] = 1. / 6;
		c[3][0] = 2. / 9;
		c[4][0] = 2. / 3;
		c[5][0] = 1.;
		c[6][0] = 8. / 9;
		c[7][0] = 1.;

		b_main = Matrix<double>(8, 1);
		b_main[0][0] = 3. / 80;
		b_main[1][0] = 0.;
		b_main[2][0] = 4. / 25;
		b_main[3][0] = 243. / 1120;
		b_main[4][0] = 77. / 160;
		b_main[5][0] = 73. / 700;
		b_main[6][0] = 0.;
		b_main[7][0] = 0.;

		b_subs = Matrix<double>(8, 1);
		b_subs[0][0] = 57. / 640;
		b_subs[1][0] = 0.;
		b_subs[2][0] = -16. / 65;
		b_subs[3][0] = 1377. / 2240;
		b_subs[4][0] = 121. / 320;
		b_subs[5][0] = 0.;
		b_subs[6][0] = 891. / 8320;
		b_subs[7][0] = 2. / 35;
	}
	Matrix<double> execute(double t_0, Matrix<double>& y_0, double h, Ode* f)
	{
		std::vector<Matrix<double>> k;
		Matrix<double> tmp_y(y_0.get_amount_of_lines(), 1);
		k.push_back((*f)(t_0, y_0));
		for (size_t i = 1; i < 8; i++)
		{
			tmp_y = tmp_y * 0;
			for (size_t j = 0; j < i; j++)
			{
				tmp_y = tmp_y + A[i][j] * k[j];
			}
			k.push_back((*f)(t_0 + c[i][0] * h, y_0 + tmp_y * h));
		}
		Matrix<double> tmp_acc(y_0.get_amount_of_lines(), 1, 0.);
		tmp_y = y_0;
		tmp_acc = y_0;
		for (size_t i = 0; i < 8; i++)
		{
			tmp_y = tmp_y + h * b_main[i][0] * k[i];
			tmp_acc = tmp_acc + h * b_subs[i][0] * k[i];
		}

		Matrix<double> ttmp_y(tmp_y.get_amount_of_lines() + 1, 1);
		for (size_t i = 0; i < tmp_y.get_amount_of_lines(); i++)
			ttmp_y[i][0] = tmp_y[i][0];
		ttmp_y[tmp_y.get_amount_of_lines()][0] = (tmp_acc - tmp_y).mod();
		return ttmp_y;
	}
private:
	Matrix<double> A;
	Matrix<double> c;
	Matrix<double> b_main;
	Matrix<double> b_subs;
};

class RK57FEq1 // (5,2)
{
public:
	RK57FEq1()
	{
		A = Matrix<double>(7, 7);
		for (size_t i = 0; i < 7; i++)
			A[0][i] = 0.;

		A[1][0] = 2. / 9;
		for (size_t i = 1; i < 7; i++)
			A[1][i] = 0.;

		A[2][0] = 1. / 12;
		A[2][1] = 1. / 4;
		for (size_t i = 2; i < 7; i++)
			A[2][i] = 0.;

		A[3][0] = 1. / 8;
		A[3][1] = 0.;
		A[3][2] = 3. / 8;
		for (size_t i = 3; i < 7; i++)
			A[3][i] = 0.;

		A[4][0] = 91. / 500;
		A[4][1] = -27. / 100;
		A[4][2] = 78. / 125;
		A[4][3] = 8. / 125;
		for (size_t i = 4; i < 7; i++)
			A[4][i] = 0.;

		A[5][0] = -11. / 20;
		A[5][1] = 27. / 20;
		A[5][2] = 12. / 5;
		A[5][3] = -36. / 5;
		A[5][4] = 5.;
		A[5][5] = 0.;
		A[5][6] = 0.;

		A[6][0] = 1. / 12;
		A[6][1] = 0.;
		A[6][2] = 27. / 32;
		A[6][3] = -4. / 3;
		A[6][4] = 125. / 96;
		A[6][5] = 5. / 48;
		A[6][6] = 0.;

		c = Matrix<double>(7, 1);
		c[0][0] = 0.;
		c[1][0] = 2. / 9;
		c[2][0] = 1. / 3;
		c[3][0] = 1. / 2;
		c[4][0] = 3. / 5;
		c[5][0] = 1.;
		c[6][0] = 1.;

		b_main = Matrix<double>(7, 1);
		b_main[0][0] = 1. / 12;
		b_main[1][0] = 0.;
		b_main[2][0] = 27. / 32;
		b_main[3][0] = -4. / 3;
		b_main[4][0] = 125. / 96;
		b_main[5][0] = 5. / 48;
		b_main[6][0] = 0.;

		b_subs = Matrix<double>(7, 1);
		b_subs[0][0] = 2. / 15;
		b_subs[1][0] = 0.;
		b_subs[2][0] = 27. / 80;
		b_subs[3][0] = -2. / 15;
		b_subs[4][0] = 25. / 48;
		b_subs[5][0] = 1. / 24;
		b_subs[6][0] = 1. / 10;

		/*
				orders=[5;4];
				isFSAL=true;
		*/
	}
	Matrix<double> execute(double t_0, Matrix<double>& y_0, double h, Matrix<double>(*f)(double, Matrix<double>))
	{
		std::vector<Matrix<double>> k;
		Matrix<double> tmp_y(y_0.get_amount_of_lines(), 1);
		k.push_back(f(t_0, y_0));
		for (size_t i = 1; i < 7; i++)
		{
			tmp_y = tmp_y * 0;
			for (size_t j = 0; j < i; j++)
			{
				tmp_y = tmp_y + A[i][j] * k[j];
			}
			k.push_back(f(t_0 + c[i][0] * h, y_0 + tmp_y * h));
		}
		Matrix<double> tmp_acc(y_0.get_amount_of_lines(), 1, 0.);
		tmp_y = y_0;
		tmp_acc = y_0;
		for (size_t i = 0; i < 7; i++)
		{
			tmp_y = tmp_y + h * b_main[i][0] * k[i];
			tmp_acc = tmp_acc + h * b_subs[i][0] * k[i];
		}

		Matrix<double> ttmp_y(tmp_y.get_amount_of_lines() + 1, 1);
		for (size_t i = 0; i < tmp_y.get_amount_of_lines(); i++)
			ttmp_y[i][0] = tmp_y[i][0];
		ttmp_y[tmp_y.get_amount_of_lines()][0] = (tmp_acc - tmp_y).mod();
		return ttmp_y;
	}
private:
	Matrix<double> A;
	Matrix<double> c;
	Matrix<double> b_main;
	Matrix<double> b_subs;
};

class RK68M // (6,2)
{
public:
	RK68M()
	{
		A = Matrix<double>(8, 8);
		for (size_t i = 0; i < 8; i++)
			A[0][i] = 0.;

		A[1][0] = 1. / 10;
		for (size_t i = 1; i < 8; i++)
			A[1][i] = 0.;

		A[2][0] = -2. / 81;
		A[2][1] = 20. / 91;
		for (size_t i = 2; i < 8; i++)
			A[2][i] = 0.;

		A[3][0] = 615. / 1372;
		A[3][1] = -270./343;
		A[3][2] = 1053. / 1372;
		for (size_t i = 3; i < 8; i++)
			A[3][i] = 0.;

		A[4][0] = 3243. / 5500;
		A[4][1] = -54. / 55;
		A[4][2] = 50949. / 71500;
		A[4][3] = 4998. / 17875;
		for (size_t i = 4; i < 8; i++)
			A[4][i] = 0.;

		A[5][0] = -26492. / 37125;
		A[5][1] = 72. / 55;
		A[5][2] = 2808. / 23375;
		A[5][3] = -24206. / 37125;
		A[5][4] = 338./459;
		A[5][5] = 0.;
		A[5][6] = 0.;
		A[5][7] = 0.;

		A[6][0] = 5561. / 2376;
		A[6][1] = -35./11;
		A[6][2] = -24117. / 31603;
		A[6][3] = 899983. / 200772;
		A[6][4] = -5225. / 1836;
		A[6][5] = 3925. / 4056;
		A[6][6] = 0.;
		A[6][7] = 0.;

		A[7][0] = 465467. / 266112;
		A[7][1] = -2945. / 1232;
		A[7][2] = -5610201. / 14158144;
		A[7][3] = 10513573. / 2312352;
		A[7][4] = -424325. / 205632;
		A[7][5] = 376225. / 454272;
		A[7][6] = 0.;
		A[7][6] = 0.;

		c = Matrix<double>(7, 1);
		c[0][0] = 0.;
		c[1][0] = 1. / 10;
		c[2][0] = 2. / 9;
		c[3][0] = 3. / 7;
		c[4][0] = 3. / 5;
		c[5][0] = 4./5;
		c[6][0] = 1.;
		c[7][0] = 1.;


		b_main = Matrix<double>(7, 1);
		b_main[0][0] = 61. / 864;
		b_main[1][0] = 0.;
		b_main[2][0] = 98415. / 321776;
		b_main[3][0] = 16807. / 146016;
		b_main[4][0] = 1375. / 7344;
		b_main[5][0] = 1375. / 5408;
		b_main[6][0] = -37./1120;
		b_main[7][0] = 1. / 10;

		b_subs = Matrix<double>(7, 1);
		b_subs[0][0] = 821. / 10800;
		b_subs[1][0] = 0.;
		b_subs[2][0] = 19683. / 71825;
		b_subs[3][0] = 175273. / 912600;
		b_subs[4][0] = 395. / 3672;
		b_subs[5][0] = 758. / 2704;
		b_subs[6][0] = 3. / 50;
		b_subs[7][0] = 0.;

		/*
				orders=[6;5];
				isFSAL=false;
		*/
	}
	Matrix<double> execute(double t_0, Matrix<double>& y_0, double h, Matrix<double>(*f)(double, Matrix<double>))
	{
		std::vector<Matrix<double>> k;
		Matrix<double> tmp_y(y_0.get_amount_of_lines(), 1);
		k.push_back(f(t_0, y_0));
		for (size_t i = 1; i < 8; i++)
		{
			tmp_y = tmp_y * 0;
			for (size_t j = 0; j < i; j++)
			{
				tmp_y = tmp_y + A[i][j] * k[j];
			}
			k.push_back(f(t_0 + c[i][0] * h, y_0 + tmp_y * h));
		}
		Matrix<double> tmp_acc(y_0.get_amount_of_lines(), 1, 0.);
		tmp_y = y_0;
		tmp_acc = y_0;
		for (size_t i = 0; i < 8; i++)
		{
			tmp_y = tmp_y + h * b_main[i][0] * k[i];
			tmp_acc = tmp_acc + h * b_subs[i][0] * k[i];
		}

		Matrix<double> ttmp_y(tmp_y.get_amount_of_lines() + 1, 1);
		for (size_t i = 0; i < tmp_y.get_amount_of_lines(); i++)
			ttmp_y[i][0] = tmp_y[i][0];
		ttmp_y[tmp_y.get_amount_of_lines()][0] = (tmp_acc - tmp_y).mod();
		return ttmp_y;
	}
private:
	Matrix<double> A;
	Matrix<double> c;
	Matrix<double> b_main;
	Matrix<double> b_subs;
};

template<typename RK, typename LV>
class RKintegrator : public algorithm
{
public:
	Matrix<double> step(double t_0, Matrix<double> beg_state, double size_step, Ode* f)
	{
		RK rk;
		Matrix<double> state;
		state = rk.execute(t_0, beg_state, size_step, f);
		return state;
	}
	std::vector<Matrix<double>> n_steps(double t_0, Matrix<double> beg_state, double size_step, size_t n)
	{
		std::vector<Matrix<double>> res;
		Matrix<double> tmp_state;
		Ode* f = new LV();
		f->set_const(consts);
		res.push_back(beg_state);
		for (size_t i = 0; i < n; i++)
		{
			tmp_state = step(t_0, beg_state, size_step, f);
			for (size_t i = 0; i < beg_state.get_amount_of_lines(); i++)
			{
				beg_state[i][0] = tmp_state[i][0];
			}
			res.push_back(tmp_state);
			t_0 += size_step;
		}
		return res;
	}
	vector<Matrix<double>> do_your_job(Matrix<double> d)
	{
		vector<Matrix<double>> states;
		vector<Matrix<double>> res;
		double t_0 = d[0][0];
		double size_step = d[0][1];
		size_t n = d[0][2];
		size_t dim = d[0][3];
		for (size_t i = 4; i < d.get_amount_of_columns(); i++)
		{
			consts.push_back(d[0][i]);
		}
		for (size_t i = 1; i < d.get_amount_of_lines(); i++)
		{
			states.push_back(d.slice(i, i).slice(0, dim - 1, 1, true).transpose());
		}
		for (auto i : states)
		{
			auto k = n_steps(t_0, i, size_step, n);
			res.insert(res.end(), k.cbegin(), k.cend());
		}
		return res;
	}
	void set_consts(vector<double> v)
	{
		consts = v;
	}
private:
	vector<double> consts;
};