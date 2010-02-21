#include "stdafx.h"
#include "Matrix.h"
#include "ExprException.h"
#include <stdio.h>
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <crtdbg.h>

Matrix::Matrix(): 
_Rows(0), _Cols(0), _ValueVec()
{ }

Matrix::Matrix(Matrix::Row_Col_t rows, Matrix::Row_Col_t cols):
_Rows(rows), _Cols(cols), _ValueVec(rows * cols, 0.0)
{ }

Matrix::Matrix(Matrix::Row_Col_t rows, Matrix::Row_Col_t cols, Matrix::RealVal_t *realValArr):
_Rows(rows), _Cols(cols), _ValueVec(realValArr, realValArr + rows*cols)
{ }

Matrix::Matrix(Matrix::RowCol_t rowcol, Matrix::RealValVec_t &realValVecRef):
_Rows(rowcol._Rows), _Cols(rowcol._Cols), _ValueVec()
{
    _ValueVec.swap(realValVecRef);
}

Matrix& Matrix::DotAssignmentOperator(const Matrix &matrix, char operCh)
{
    if (matrix._Rows == _Rows && matrix._Cols == _Cols) {
        Row_Col_t valueVecLen = _Rows * _Cols;
        switch (operCh) {
            case '+' :
                for (Row_Col_t i = 0; i < valueVecLen; ++i)
                    _ValueVec[i] += matrix._ValueVec[i];
                break;
            case '-' :
                for (Row_Col_t i = 0; i < valueVecLen; ++i)
                    _ValueVec[i] -= matrix._ValueVec[i];
                break;
            case '*' :
                for (Row_Col_t i = 0; i < valueVecLen; ++i)
                    _ValueVec[i] *= matrix._ValueVec[i];
                break;
            case '/' :
                for (Row_Col_t i = 0; i < valueVecLen; ++i)
                    _ValueVec[i] /= matrix._ValueVec[i];
                break;
        }
    }
    else {
        // TODO: refine
        throw ExprException("The size of another matrix must be same with target matrix.");
    }

    return *this;
}

Matrix& Matrix::DotAssignmentOperator(Matrix::RealVal_t realVal, char operCh)
{
    Row_Col_t valueVecLen = _Rows * _Cols;
    switch (operCh) {
        case '+' :
            for (Row_Col_t i = 0; i < valueVecLen; ++i)
                _ValueVec[i] += realVal;
            break;
        case '-' :
            for (Row_Col_t i = 0; i < valueVecLen; ++i)
                _ValueVec[i] -= realVal;
            break;
        case '*' :
            for (Row_Col_t i = 0; i < valueVecLen; ++i)
                _ValueVec[i] *= realVal;
            break;
        case '/' :
            for (Row_Col_t i = 0; i < valueVecLen; ++i)
                _ValueVec[i] /= realVal;
            break;
    }

    return *this;
}

void Matrix::Swap(Matrix &matrix)
{
    RowCol_t tempRowCol;

    tempRowCol._Cols = _Cols;
    tempRowCol._Rows = _Rows;
    _Cols = matrix._Cols;
    _Rows = matrix._Rows;
    matrix._Cols = tempRowCol._Cols;
    matrix._Rows = tempRowCol._Rows;
    _ValueVec.swap(matrix._ValueVec);
}

void Matrix::Swap(Matrix::RowCol_t rowcol, Matrix::RealValVec_t &realValVecRef)
{
    RowCol_t tempRowCol;

    tempRowCol._Cols = _Cols;
    tempRowCol._Rows = _Rows;
    _Cols = rowcol._Cols;
    _Rows = rowcol._Rows;
    rowcol._Cols = tempRowCol._Cols;
    rowcol._Rows = tempRowCol._Rows;
    _ValueVec.swap(realValVecRef);
}

Matrix& Matrix::operator *=(const Matrix &matrix)
{
    if (matrix._Rows == _Cols) {
        Row_Col_t resultRows = _Rows;
        Row_Col_t resultCols = matrix._Cols;
        vector<RealVal_t> resultVec(resultRows * resultCols, 0.0);

        for (Row_Col_t r = 0; r < resultRows; ++r) {
            for (Row_Col_t c = 0; c < resultCols; ++c) {
                vector<RealVal_t>::const_iterator iter_1 = _ValueVec.begin() + r*_Cols;
                vector<RealVal_t>::const_iterator iter_2 = matrix._ValueVec.begin() + c;
                RealVal_t val = (*iter_1) * (*iter_2);
                for (Row_Col_t i = 1; i < _Cols; ++i) {
                    ++iter_1;
                    iter_2 += matrix._Cols;
                    val += (*iter_1) * (*iter_2);
                }
                resultVec[r*resultCols + c] = val;
            }
        }

        this->_Cols = resultCols;
        this->_Rows = resultRows;
        this->_ValueVec.swap(resultVec);
    }
    else {
        // TODO: refine
        throw ExprException("The Rows of another matrix must be same with Cols of target matrix.");
    }

    return *this;
}

Matrix operator+(const Matrix &matrix1, const Matrix &matrix2)
{
    Matrix resultMatrix(matrix1);

    resultMatrix += matrix2;

    return resultMatrix;
}

Matrix operator-(const Matrix &matrix1, const Matrix &matrix2)
{
    Matrix resultMatrix(matrix1);

    resultMatrix -= matrix2;

    return resultMatrix;
}

Matrix operator*(const Matrix &matrix1, const Matrix &matrix2)
{
    Matrix resultMatrix(matrix1);

    resultMatrix *= matrix2;

    return resultMatrix;
}

string Matrix::ToString() const
{
    string tempStr;
    string resultStr;

    string::size_type maxValueLen = 0;
    for (Row_Col_t r = 0; r < _Rows; ++r) {
        vector<RealVal_t>::const_iterator valIter = _ValueVec.begin() + r*_Cols;
        for (Row_Col_t c = 0; c < _Cols; ++c) {
            char buffer[20];
            _ASSERT(sprintf(buffer, "%f", *valIter) < sizeof(buffer) / sizeof(char));
            char *chPtr = buffer + strlen(buffer);
            while ('0' == *(--chPtr))
                ;
            if ('.' == *chPtr)
                *chPtr = '\0';
            else
                *(++chPtr) = '\0';

            tempStr.append(buffer);
            tempStr.append(1, '|');
            if (strlen(buffer) > maxValueLen) maxValueLen = strlen(buffer);

            ++valIter;
        }
    }

    // Generate final result string;
    ++maxValueLen;
    Row_Col_t valueStrIdx = 0;
    for (string::const_iterator strIter_1 = tempStr.begin(); strIter_1 != tempStr.end(); ) {
        string::const_iterator strIter_2 = strIter_1;
        while ('|' != *strIter_2)
            ++strIter_2;

        resultStr.append(maxValueLen - (strIter_2 - strIter_1), ' ');
        resultStr.append(strIter_1, strIter_2);

        if (0 == (++valueStrIdx % _Cols))
            resultStr.append(1, '\n');

        strIter_1 = strIter_2 + 1;
    }
    resultStr.erase(resultStr.length() - 1, 1);

    return resultStr;
}