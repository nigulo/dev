// Class automatically generated by Dev-C++ New Class wizard

#ifndef MATRIX_H
#define MATRIX_H

#include "base/object.h" // inheriting class's header file
#include "vector.h"

using namespace base;

namespace engine3d {
/*
 * Implements matrix algebra
 */
class Matrix : public Object
{
	public:
		Matrix(int dim = 3);
		Matrix(int rows, int columns);
		Matrix(const Matrix& m);
		virtual ~Matrix();
		void operator=(const Matrix& m);
		void SetRow(int index, const Vector& row);
		void SetColumn(int index, const Vector& column);
		void Set(int row, int col, double d);
		double Get(int row, int col) const;
		Matrix operator+(const Matrix& m) const;
		Matrix operator-(const Matrix& m) const;
		Matrix operator*(const Matrix& m) const;
		Matrix operator*(double k) const;
		/**
		 * Multiplies the matrix with a column vector
		 */
		Vector operator*(const Vector& v) const;
		Matrix Transpose() const;
		const double* GetElements() const;
		static Matrix GetUnit(int dim = 3);
	protected:
        int numRows;
        int numColumns;
        double* elements;
        //vector<vector<double>*> elements;
};
}
#endif // MATRIX_H
