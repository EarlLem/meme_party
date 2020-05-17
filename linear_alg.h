#pragma once
#include <iostream>
#include <exception>
#include <ostream>
#include <vector>

using namespace std;

template<typename T>
class Matrix
{
private:
	vector<T> data_;
	size_t n_lines_;
	size_t n_columns_;
	size_t length_;

	class MatrixLine
	{
	public:
		MatrixLine(size_t line_ind, size_t cols, vector<T>& data) :
			line_index(line_ind),
			n_columns_(cols),
			data_(data) {}
		T& operator[](size_t index)
		{
			if ((index >= n_columns_) || (index < 0))
			{
				throw std::out_of_range("Index out of range!");
			}
			return data_[line_index * n_columns_ + index];
		}
		T operator[](size_t index) const
		{
			if ((index >= n_columns_) || (index < 0))
			{
				throw std::out_of_range("Index out of range!");
			}
			return data_[line_index * n_columns_ + index];
		}
	private:
		size_t line_index;
		size_t n_columns_;
		vector<T>& data_;
	};
public:
	Matrix() : n_lines_(0), n_columns_(0), length_(0) {}
	Matrix(size_t lines, size_t columns, T element = 0)
	{
		if (lines > 0 && columns > 0)
		{
			n_lines_ = lines;
			n_columns_ = columns;
			length_ = lines * columns;
			data_ = vector<T>(length_, element);
		}
		else
		{
			throw std::out_of_range("Index out of range!");
		}
	}
	Matrix(const Matrix& mat) :
		n_lines_(mat.n_lines_),
		n_columns_(mat.n_columns_),
		length_(n_lines_ * n_columns_)
	{
		for (const auto& i : mat)
		{
			data_.push_back(i);
		}
	}
	size_t count_lines() const
	{
		return n_lines_;
	}
	size_t count_columns() const
	{
		return n_columns_;
	}
	Matrix transpose() const
	{
		Matrix<T> tmp = Matrix<T>(n_columns_, n_lines_);
		for (size_t i = 0; i < n_lines_; i++)
		{
			for (size_t j = 0; j < n_columns_; j++)
				tmp[j][i] = data_[i * n_columns_ + j];
		}
		return tmp;
	}
	Matrix& operator=(const Matrix& mat)
	{
		if (&mat == this)
		{
			return *this;
		}
		this = mat;
	}
	Matrix map(T(*func)(T a)) const
	{
		Matrix<T> tmp{ n_lines_, n_columns_ };
		for (size_t i = 0; i < n_lines_; i++)
		{
			for (size_t j = 0; j < n_columns_; j++)
			{
				tmp[i][j] = func(this[i][j]);
			}
		}
		return tmp;
	}
	Matrix apply(T(*func)(T a))
	{
		for (size_t i = 0; i < length_; i++)
			data_[i] = func(data_[i]);
		return *this;
	}
	MatrixLine operator[] (size_t index)
	{
		if ((index >= n_lines_) || (index < 0))
		{
			throw std::out_of_range("Index out of range!");
		}
		return { index, n_columns_, data_ };
	}
};

template <class T>
std::ostream& operator<<(std::ostream& out, Matrix<T>& mat)
{
	size_t lines = mat.count_lines();
	size_t columns = mat.count_columns();
	for (size_t i = 0; i < lines; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			out << mat[i][j] << " ";
		}
		out << "\b ";
		if (i != lines - 1)
		{
			out << std::endl;
		}
	}

	return out;
}