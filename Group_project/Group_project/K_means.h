#pragma once
#include "linear_alg.h"
#include <iterator>
#include <algorithm>
#include "Threads.h"

double distance(Matrix<double> a, Matrix<double> b)//расстояние между точками
{
	double dist;
	dist = (a.transpose() - b.transpose()).mod();
	return dist;
}

class K_MEANS : public algorithm
{
public:
	K_MEANS()
	{
		_k = 0;
		_dots = Matrix<double>(0, 0);
	}

	vector<Matrix<double>> do_your_job(Matrix<double> data)
	{
		_k = static_cast<int>(data[0][0]);
		_dots = data.slice(1, data.get_amount_of_lines() - 1);

		bool cont = true;//параметр отвечающий за продолжение выполнения алгоритма
		int amount = _dots.get_amount_of_lines();//количество точек
		std::vector<Matrix<double>> centroids(_k);//вектор центроид
		std::vector<double> nrst_cent(amount);//вектор в котором обозначается ближайшая цетроида для каждой точки


		for (size_t i = 0; i < _k; i++)//случайная первичная инициализация центроид
		{
			centroids[i] = _dots.slice(i, i);
		}

		while (cont)
		{
			std::vector<size_t> size_of_clusters(_k);//количество точек в каждом кластере
			std::vector<Matrix<double>> new_centroids(_k);//вектор пересчитанных центроид

			for (size_t i = 0; i < amount; i++)//распределение точек по кластерам
			{
				std::vector<double> dists;//расстояние от точки до каждой из центроид
				for (auto c : centroids)
				{
					dists.push_back(distance(c, _dots.slice(i, i)));
				}
				nrst_cent[i] = static_cast<int>(std::distance(dists.begin(), min_element(dists.begin(), dists.end())));//определение точки в кластер
				size_of_clusters[nrst_cent[i]] += 1;//увеличение кластера
			}

			for (size_t i = 0; i < _k; i++)
			{
				new_centroids[i] = Matrix<double>(1, _dots.get_amount_of_columns());
			}

			for (size_t i = 0; i < amount; i++)
			{
				new_centroids[nrst_cent[i]] = new_centroids[nrst_cent[i]] + _dots.slice(i, i);
			}

			for (size_t i = 0; i < _k; i++)//пересчет центроид
			{
				if (size_of_clusters[i])
				{
					new_centroids[i] = new_centroids[i] / size_of_clusters[i];
				}
				else
				{
					new_centroids[i] = centroids[i];
				}
			}

			cont = false;

			for (size_t i = 0; i < _k; i++)
			{
				double shift = distance(centroids[i], new_centroids[i]);//расчет смещения центроид
				if (shift != 0)//если они сместились то процесс продолжается
				{
					cont = true;
				}
			}

			centroids = new_centroids;//переопределение центроид

		}

	}
	string shout() 
	{
		return "K_means";
	}
private:
	int _k;
	Matrix<double> _dots;
};