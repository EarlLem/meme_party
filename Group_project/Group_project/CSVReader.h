#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "linear_alg.h"


class CSVRow
{
public:
	std::string const& operator[](std::size_t index) const
	{
		return m_data[index];
	}
	std::size_t size() const
	{
		return m_data.size();
	}
	void readNextRow(std::istream& str)
	{
		std::string line;
		std::getline(str, line);

		std::stringstream lineStream(line);
		std::string cell;

		m_data.clear();
		while (std::getline(lineStream, cell, ','))
		{
			m_data.push_back(cell);
		}
		if (!lineStream && cell.empty())
		{
			m_data.push_back("");
		}
	}
	friend std::ostream& operator<<(std::ostream& out, CSVRow& f)// to run tests of CSV lib
	{
		for (size_t i = 0; i < f.size(); i++)
			out << f.m_data[i] << " ";
		return out;
	}
	friend std::istream& operator>>(std::istream& str, CSVRow& data)
	{
		data.readNextRow(str);
		return str;
	}
	template<typename T>
	friend Matrix<T> CSVtoMatrix(std::string file_name);

private:
	std::vector<std::string> m_data;
};

template<typename T>
Matrix<T>& CSVtoMatrix(std::string file_name)
{
  std::ifstream file(file_name);
  std::vector<CSVRow> data;
  CSVRow row;

  while (file >> row)
  {
    data.push_back(row);
  }
  size_t n_lines = data.size();
  size_t n_columns = data[0].size();//в первой строчке название, во второй коэффициенты


  Matrix<T> tmp(n_lines - 2, n_columns);
  for (size_t i = 2; i < n_lines; i++)
  {
    for (size_t j = 0; j < n_columns; j++)
    {
      tmp[i - 2][j] = std::stod(data[i][j]);
    }
  }

  return *tmp;
}

template <typename T>
void MatrixtoCSV(std::string file_name, Matrix<T> mat)
{
	std::ofstream myfile;
	myfile.open(file_name);
	size_t strok = mat.get_amount_of_lines();
	size_t stolb = mat.get_amount_of_columns();
	for (size_t i = 0; i < strok; i++)
	{
		for (size_t j = 0; j < stolb; j++)
		{
			myfile << std::to_string(mat[i][j]) << ",";
		}
		myfile << "\n";
	}
}


template <typename T>
void write_in_file(std::string file_name, std::vector<Matrix<T>> res)
{
	std::ofstream myfile;
	myfile.open(file_name);
	for (size_t i = 0; i < res.size(); i++)
	{
		for (size_t j = 0; j < res[i].get_amount_of_lines() - 1; j++)
			myfile << std::to_string(res[i][j][0]) << ",";
		myfile << std::to_string(res[i][res[i].get_amount_of_lines() - 1][0]) << "\n";
	}
	myfile.close();
}