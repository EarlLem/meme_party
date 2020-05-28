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
		for (const auto& i : mat.data_)
		{
			data_.push_back(i);
		}
	}
	size_t get_amount_of_lines() const
	{
		return n_lines_;
	}
	size_t get_amount_of_columns() const
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
		n_lines_ = mat.n_lines_;
		n_columns_ = mat.n_columns_;
		length_ = mat.length_;
		data_ = vector<T>(length_);
		for (size_t i = 0; i < length_; i++)
			data_[i] = mat.data_[i];
		return *this;
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
	friend Matrix operator+(Matrix a, Matrix b)
	{
		if (a.get_amount_of_columns() == b.get_amount_of_columns() && a.get_amount_of_lines() == b.get_amount_of_lines())
		{
			Matrix tmp(a.get_amount_of_lines(), a.get_amount_of_columns());
			for (size_t i = 0; i < a.get_amount_of_lines(); i++)
			{
				for (size_t j = 0; j < a.get_amount_of_columns(); j++)
				{
					tmp[i][j] = a[i][j] + b[i][j];
				}
			}
			return tmp;
		}
		else
			throw std::out_of_range("Wrong Matrix sizes!");
	}
	friend Matrix operator+(T a, Matrix b)
	{
		Matrix tmp(b.get_amount_of_lines(), b.get_amount_of_columns());
		for (size_t i = 0; i < b.get_amount_of_lines(); i++)
		{
			for (size_t j = 0; j < b.get_amount_of_columns(); j++)
			{
				tmp[i][j] = a + b[i][j];
			}
		}
		return tmp;
	}
	friend Matrix operator+(Matrix a, T b)
	{
		Matrix tmp(a.get_amount_of_lines(), a.get_amount_of_columns());
		for (size_t i = 0; i < a.get_amount_of_lines(); i++)
		{
			for (size_t j = 0; j < a.get_amount_of_columns(); j++)
			{
				tmp[i][j] = a[i][j] + b;
			}
		}
		return tmp;
	}
	friend Matrix operator-(Matrix a, Matrix b)
	{
		if (a.get_amount_of_columns() == b.get_amount_of_columns() && a.get_amount_of_lines() == b.get_amount_of_lines())
		{
			Matrix tmp(a.get_amount_of_lines(), a.get_amount_of_columns());
			for (size_t i = 0; i < a.get_amount_of_lines(); i++)
			{
				for (size_t j = 0; j < a.get_amount_of_columns(); j++)
				{
					tmp[i][j] = a[i][j] - b[i][j];
				}
			}
			return tmp;
		}
		else
			throw std::out_of_range("Wrong Matrix sizes!");
	}
	friend Matrix operator-(T a, Matrix b)
	{
		Matrix tmp(b.get_amount_of_lines(), b.get_amount_of_columns());
		for (size_t i = 0; i < b.get_amount_of_lines(); i++)
		{
			for (size_t j = 0; j < b.get_amount_of_columns(); j++)
			{
				tmp[i][j] = a - b[i][j];
			}
		}
		return tmp;
	}
	friend Matrix operator-(Matrix a, T b)
	{
		Matrix tmp(a.get_amount_of_lines(), a.get_amount_of_columns());
		for (size_t i = 0; i < a.get_amount_of_lines(); i++)
		{
			for (size_t j = 0; j < a.get_amount_of_columns(); j++)
			{
				tmp[i][j] = a[i][j] - b;
			}
		}
		return tmp;
	}
	friend Matrix operator*(Matrix a, Matrix b)
	{
		if (b.get_amount_of_columns() == 1 && a.get_amount_of_columns() == 1 && a.get_amount_of_lines() == b.get_amount_of_lines())
		{
			Matrix mat(1, 1);
			for (size_t i = 0; i < a.get_amount_of_lines(); i++)
				mat[0][0] += a[i][0] * b[i][0];
			return mat;
		}
		if (a.get_amount_of_columns() == b.get_amount_of_lines())
		{
			size_t size_lines = a.get_amount_of_lines();
			size_t size_columns = b.get_amount_of_columns();
			Matrix mat(size_lines, size_columns);
			for (size_t i = 0; i < size_lines; i++)
			{
				for (size_t j = 0; j < size_columns; j++)
				{
					for (size_t k = 0; k < a.get_amount_of_columns(); k++)
						mat[i][j] += a[i][k] * b[k][j];
				}
			}
			return mat;
		}
		else
		{
			throw std::out_of_range("Sizes do not match!");
		}
	}
	Matrix slice(size_t begin, size_t end, size_t step = 1, bool columns = false) const
	{
		if (!columns)
		{
			size_t b_size = (end - begin + 1) / step;
			Matrix tmp(b_size, n_columns_);
			size_t i_n = 0;
			for (size_t i = begin; i <= end; i += step)
			{
				for (size_t j = 0; j < n_columns_; j++)
				{
					tmp.data_[i_n * n_columns_ + j] = data_[i * n_columns_ + j];
				}
				i_n++;
			}
			return tmp;
		}
		else
		{
			size_t b_size = (end - begin + 1) / step;
			Matrix tmp(n_lines_, b_size);
			size_t i_n = 0;
			for (size_t i = 0; i < n_lines_; i++)
			{
				for (size_t j = begin; j <= end; j += step)
				{
					tmp.data_[i * b_size + i_n] = data_[i * n_columns_ + j];
					i_n++;
				}
				i_n = 0;
			}
			return tmp;
		}
	}
	friend Matrix operator*(Matrix a, T b)
	{
		size_t size_lines = a.get_amount_of_lines();
		size_t size_columns = a.get_amount_of_columns();
		for (size_t i = 0; i < size_lines; i++)
		{
			for (size_t j = 0; j < size_columns; j++)
			{
				a[i][j] = a[i][j] * b;
			}
		}
		return a;
	}
	friend Matrix operator*(T b, Matrix a)
	{
		size_t size_lines = a.get_amount_of_lines();
		size_t size_columns = a.get_amount_of_columns();
		for (size_t i = 0; i < size_lines; i++)
		{
			for (size_t j = 0; j < size_columns; j++)
			{
				a[i][j] = a[i][j] * b;
			}
		}
		return a;
	}
	friend Matrix operator/(Matrix a, T b)
	{
		if (b != 0)
		{

			for (size_t i = 0; i < a.get_amount_of_lines(); i++)
			{
				for (size_t j = 0; j < a.get_amount_of_columns(); j++)
				{
					a[i][j] = a[i][j] / b;
				}
			}
			return a;
		}
		else
			throw std::out_of_range("Division by zero!");
	}
	friend Matrix operator/(Matrix a, Matrix b)
	{
		if (a.get_amount_of_columns() == b.get_amount_of_columns() && a.get_amount_of_lines() == b.get_amount_of_lines())
		{
			Matrix tmp(a.get_amount_of_lines(), a.get_amount_of_columns());
			for (size_t i = 0; i < a.get_amount_of_lines(); i++)
			{
				for (size_t j = 0; j < a.get_amount_of_columns(); j++)
				{
					if (b[i][j] != 0)
						tmp[i][j] = a[i][j] / b[i][j];
					else
						std::out_of_range("Division by zero!");
				}
			}
			return tmp;
		}
		else
			throw std::out_of_range("Wrong Matrix sizes!");
	}
	T mod()
	{
		T sum = 0.;
		if (n_columns_ == 1)
		{
			for (size_t j = 0; j < length_; j++)
			{
				sum += data_[j] * data_[j];
			}
			return sqrt(sum);
		}
		else
		{
			throw std::out_of_range("Ivalid input!");
		}
	}
};

template <class T>
std::ostream& operator<<(std::ostream& out, Matrix<T>& mat)
{
	size_t lines = mat.get_amount_of_lines();
	size_t columns = mat.get_amount_of_columns();
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