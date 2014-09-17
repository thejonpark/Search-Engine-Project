#include "matrix_functions.h"

Matrix::Matrix(size_t n)
{
    mySize = n;
    myData = new double*[n];
    for (size_t i = 0; i < n; ++ i)
    {
        myData[i] = new double[n];
    }
}

Matrix::Matrix(const Matrix& other)
    : myData(other.myData), mySize(other.mySize) 
{
    for (size_t i = 0; i < mySize; ++ i)
    {
        for (size_t j = 0; j < mySize; ++ j)
        {
            myData[i][j] = other.myData[i][j];
        }
    }
}

Matrix::~Matrix()
{
    for (size_t i = 0; i < mySize; ++ i)
    {
        delete [] myData[i];
    }
    delete [] myData;
}

double* Matrix::operator[] (size_t ind)
{
    return myData[ind];
}

void multiply(Matrix& mat, vector<double>& vec)
{
    vector<double> sub (vec);
    for(size_t i = 0; i < mat.mySize; ++ i)
    {
        vec[i] = 0;
        for(size_t j = 0; j < mat.mySize; ++ j)
        {
            vec[i] += mat[i][j] * sub[j];
        }
    }
}
