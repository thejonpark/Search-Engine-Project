#ifndef MATRIX_FUNCTIONS_H
#define MATRIX_FUNCTIONS_H

#include <cstddef>
#include <vector>
#include "web_page.h"

struct Matrix
{
    // internal data
    double** myData;
    size_t mySize;

    // construct nxn
    Matrix(size_t n);

    // copy other
    Matrix(const Matrix& other);

    // destruct
    ~Matrix();

    // access data
    double* operator[] (size_t index);
};

// access by mat[int][int] and vec[int].
// calculates mat * vec and store the ans in vec
void multiply(Matrix& mat, vector<double>& vec);

#endif
