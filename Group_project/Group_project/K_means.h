#pragma once
#include "linear_alg.h"
#include <iterator>
#include <algorithm>

double distance(Matrix<double> a, Matrix<double> b)//���������� ����� �������
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

	vector<Matrix<double>> do_your_job(Matrix<double>* data)
	{
		_k = static_cast<int>((*data)[0][0]);
		_dots = data->slice(1, data->get_amount_of_lines() - 1);

		bool cont = true;//�������� ���������� �� ����������� ���������� ���������
		int amount = _dots.get_amount_of_lines();//���������� �����
		std::vector<Matrix<double>> centroids(_k);//������ ��������
		std::vector<double> nrst_cent(amount);//������ � ������� ������������ ��������� �������� ��� ������ �����


		for (size_t i = 0; i < _k; i++)//��������� ��������� ������������� ��������
		{
			centroids[i] = _dots.slice(i, i);
		}

		while (cont)
		{
			std::vector<size_t> size_of_clusters(_k);//���������� ����� � ������ ��������
			std::vector<Matrix<double>> new_centroids(_k);//������ ������������� ��������

			for (size_t i = 0; i < amount; i++)//������������� ����� �� ���������
			{
				std::vector<double> dists;//���������� �� ����� �� ������ �� ��������
				for (auto c : centroids)
				{
					dists.push_back(distance(c, _dots.slice(i, i)));
				}
				nrst_cent[i] = static_cast<int>(std::distance(dists.begin(), min_element(dists.begin(), dists.end())));//����������� ����� � �������
				size_of_clusters[nrst_cent[i]] += 1;//���������� ��������
			}

			for (size_t i = 0; i < _k; i++)
			{
				new_centroids[i] = Matrix<double>(1, _dots.get_amount_of_columns());
			}

			for (size_t i = 0; i < amount; i++)
			{
				new_centroids[nrst_cent[i]] = new_centroids[nrst_cent[i]] + _dots.slice(i, i);
			}

			for (size_t i = 0; i < _k; i++)//�������� ��������
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
				double shift = distance(centroids[i], new_centroids[i]);//������ �������� ��������
				if (shift != 0)//���� ��� ���������� �� ������� ������������
				{
					cont = true;
				}
			}

			centroids = new_centroids;//��������������� ��������

		}

	}
	string shout() {}
private:
	int _k;
	Matrix<double> _dots;
};