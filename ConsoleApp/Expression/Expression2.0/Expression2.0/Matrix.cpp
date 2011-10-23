#include "stdafx.h"
#include "Matrix.h"
#include "ExprException.h"
#include "DoubleComparison.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <crtdbg.h>
using namespace std;

Matrix::Matrix() : 
_Rows(0), _Cols(0), _ValueVec()
{ }

Matrix::Matrix(Matrix::Row_Col_t rows, Matrix::Row_Col_t cols) :
_Rows(rows), _Cols(cols), _ValueVec(rows * cols, 0.0)
{ }

Matrix::Matrix(Matrix::Row_Col_t rows, Matrix::Row_Col_t cols, char signCh) :
_Rows(rows), _Cols(cols), _ValueVec(rows * cols, 0.0)
{
    if ('e' == signCh || 'E' == signCh) {
        if (rows == cols) {
        Row_Col_t increment = cols + 1;

        for (Row_Col_t idx = 0; idx < _ValueVec.size(); idx += increment)
            _ValueVec[idx] = 1.0;
        }
        else {
            throw ExprException("E Matrix must be a square Matrix.");
        }
    }
    else { 
        _ASSERT(0); // unknow sign char
    }
}

Matrix::Matrix(Matrix::Row_Col_t rows, Matrix::Row_Col_t cols, Matrix::RealVal_t *realValArr) :
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
                {
                    if (DOUBLE_NEQZ(matrix._ValueVec[i]))
                        _ValueVec[i] /= matrix._ValueVec[i];
                    else
                        throw ExprException("The right matrix of './' (dot divide) contains Zero.");
                }
                break;
        }
    }
    else {
        // TODO: refine
        throw ExprException("The size of two operand matrixs must be equal.");
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

Matrix& Matrix::operator*=(const Matrix &matrix)
{
    Matrix multiplyRet;
    string exprExStr;

    if (multiplyRet.MultiplyInto(*this, matrix, &exprExStr))
        this->Swap(multiplyRet);
    else
        throw ExprException(exprExStr.c_str());

    return (*this);
}

Matrix& Matrix::operator/=(const Matrix &matrix)
{
    Matrix invMatrix(matrix);
    string exprExStr;

    if (!invMatrix.InversionTransToC(this, &exprExStr))
        throw ExprException(exprExStr.c_str());

    return (*this);
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
    Matrix multiplyRet;
    string exprExStr;

    if (!multiplyRet.MultiplyInto(matrix1, matrix2, &exprExStr))
        throw ExprException(exprExStr.c_str());

    return multiplyRet;
}

Matrix& Matrix::ToInversion()
{
    if (_Rows != _Cols)
        throw ExprException("Inversion Matrix just can be compute on a square Matrix.");

    Matrix invMatrix(_Rows, _Cols, 'E');
    string exprExStr;

    if (!this->InversionTransToC(&invMatrix, &exprExStr))
        throw ExprException(exprExStr.c_str());
    this->Swap(invMatrix);

    return *this;
}

bool Matrix::InversionTransToR(Matrix *pInvMatrix, std::string *pErrStr)
{
    _ASSERT(NULL != pInvMatrix);
    _ASSERT(this->_Rows == this->_Cols);

    if (this->_Rows == pInvMatrix->_Rows) {
        // Clear below triangle
        for (Row_Col_t rcIdx = 0; rcIdx < (_Rows - 1); ++rcIdx) {
            if (DOUBLE_EQZ(this->At(rcIdx, rcIdx))) {
                for (Row_Col_t rIdx = (rcIdx + 1); rIdx < _Rows; ++rIdx) {
                    if (DOUBLE_NEQZ(this->At(rIdx, rcIdx))) {
                        this->ElementaryTransferR(rcIdx, rIdx, 0.0);
                        pInvMatrix->ElementaryTransferR(rcIdx, rIdx, 0.0);
                        break;
                    }
                }
                if (DOUBLE_EQZ(this->At(rcIdx, rcIdx))) {
                    if (NULL != pErrStr)
                        pErrStr->assign("The rank value of this Matrix is Zero.");

                    return false;
                }
            }
            for (Row_Col_t rIdx = (rcIdx + 1); rIdx < _Rows; ++rIdx) {
                if (DOUBLE_NEQZ(this->At(rIdx, rcIdx))) {
                    RealVal_t multiple = this->At(rIdx, rcIdx)/this->At(rcIdx, rcIdx)*-1.0;

                    this->ElementaryTransferR(rcIdx, rIdx, multiple);
                    pInvMatrix->ElementaryTransferR(rcIdx, rIdx, multiple);
                }
            }
        }
        if (DOUBLE_EQZ(this->At(_Rows - 1, _Cols - 1))) {
            if (NULL != pErrStr)
                pErrStr->assign("The rank value of this Matrix is Zero.");

            return false;
        }
        // Clear above triangle
        for (Row_Col_t rcIdx = 1; rcIdx < _Rows; ++rcIdx) {
            for (Row_Col_t rIdx = 0; rIdx < rcIdx; ++rIdx) {
                if (DOUBLE_NEQZ(this->At(rIdx, rcIdx))) {
                    RealVal_t multiple = this->At(rIdx, rcIdx)/this->At(rcIdx, rcIdx)*-1.0;

                    this->ElementaryTransferR(rcIdx, rIdx, multiple);
                    pInvMatrix->ElementaryTransferR(rcIdx, rIdx, multiple);
                }
            }
        }
        // transfer all these number to 1.0
        for (Row_Col_t rcIdx = 0; rcIdx < _Rows; ++rcIdx) {
            if (DOUBLE_NEQ(this->At(rcIdx, rcIdx), 1.0)) {
                RealVal_t multiple = 1.0/this->At(rcIdx, rcIdx);

                this->ElementaryTransferR(rcIdx, rcIdx, multiple);
                pInvMatrix->ElementaryTransferR(rcIdx, rcIdx, multiple);
            }
        }
    }
    else {
        if (NULL != pErrStr)
            pErrStr->assign("Row count of target Matrix must be equal with current Matrix.");

        return false;
    }

    return true;
}

bool Matrix::InversionTransToC(Matrix *pInvMatrix, std::string *pErrStr)
{
    _ASSERT(NULL != pInvMatrix);
    _ASSERT(this->_Rows == this->_Cols);

    if (this->_Cols == pInvMatrix->_Cols) {
        // Clear right triangle
        for (Row_Col_t rcIdx = 0; rcIdx < (_Cols - 1); ++rcIdx) {
            if (DOUBLE_EQZ(this->At(rcIdx, rcIdx))) {
                for (Row_Col_t cIdx = (rcIdx + 1); cIdx < _Cols; ++cIdx) {
                    if (DOUBLE_NEQZ(this->At(rcIdx, cIdx))) {
                        this->ElementaryTransferC(rcIdx, cIdx, 0.0);
                        pInvMatrix->ElementaryTransferC(rcIdx, cIdx, 0.0);
                        break;
                    }
                }
                if (DOUBLE_EQZ(this->At(rcIdx, rcIdx))) {
                    if (NULL != pErrStr)
                        pErrStr->assign("The rank value of this Matrix is Zero.");

                    return false;
                }
            }
            for (Row_Col_t cIdx = (rcIdx + 1); cIdx < _Cols; ++cIdx) {
                if (DOUBLE_NEQZ(this->At(rcIdx, cIdx))) {
                    RealVal_t multiple = this->At(rcIdx, cIdx)/this->At(rcIdx, rcIdx)*-1.0;

                    this->ElementaryTransferC(rcIdx, cIdx, multiple);
                    pInvMatrix->ElementaryTransferC(rcIdx, cIdx, multiple);
                }
            }
        }
        if (DOUBLE_EQZ(this->At(_Rows - 1, _Cols - 1))) {
            if (NULL != pErrStr)
                pErrStr->assign("The rank value of this Matrix is Zero.");

            return false;
        }
        // Clear left triangle
        for (Row_Col_t rcIdx = 1; rcIdx < _Cols; ++rcIdx) {
            for (Row_Col_t cIdx = 0; cIdx < rcIdx; ++cIdx) {
                if (DOUBLE_NEQZ(this->At(rcIdx, cIdx))) {
                    RealVal_t multiple = this->At(rcIdx, cIdx)/this->At(rcIdx, rcIdx)*-1.0;

                    this->ElementaryTransferC(rcIdx, cIdx, multiple);
                    pInvMatrix->ElementaryTransferC(rcIdx, cIdx, multiple);
                }
            }
        }
        // transfer all these number to 1.0
        for (Row_Col_t rcIdx = 0; rcIdx < _Cols; ++rcIdx) {
            if (DOUBLE_NEQ(this->At(rcIdx, rcIdx), 1.0)) {
                RealVal_t multiple = 1.0/this->At(rcIdx, rcIdx);

                this->ElementaryTransferC(rcIdx, rcIdx, multiple);
                pInvMatrix->ElementaryTransferC(rcIdx, rcIdx, multiple);
            }
        }
    }
    else {
        if (NULL != pErrStr)
            pErrStr->assign("Column count of target Matrix must be equal with current Matrix.");

        return false;
    }

    return true;
}

bool Matrix::MultiplyInto(const Matrix &matrixL, const Matrix &matrixR, string *pErrStr)
{
    if (matrixL._Cols == matrixR._Rows) {
        Row_Col_t resultRows = matrixL._Rows;
        Row_Col_t resultCols = matrixR._Cols;
        Row_Col_t resultVecLen = resultRows * resultCols;

        if (this->_ValueVec.size() != resultVecLen)
            this->_ValueVec = RealValVec_t(resultVecLen, 0.0);
        this->_Rows = resultRows;
        this->_Cols = resultCols;

        for (Row_Col_t r = 0; r < resultRows; ++r) {
            for (Row_Col_t c = 0; c < resultCols; ++c) {
                RealValCIter_t iter_1 = matrixL._ValueVec.begin() + r * matrixL._Cols;
                RealValCIter_t iter_2 = matrixR._ValueVec.begin() + c;
                RealVal_t val = (*iter_1) * (*iter_2);
                for (Row_Col_t i = 1; i < matrixL._Cols; ++i) {
                    ++iter_1;
                    iter_2 += matrixR._Cols;
                    val += (*iter_1) * (*iter_2);
                }
                this->_ValueVec[r*resultCols + c] = val;
            }
        }

        return true;
    }
    else {
        if (NULL != pErrStr)
            pErrStr->assign("The Cols of left matrix must be same with Rows of right matrix.");

        return false;
    }
}

void Matrix::ElementaryTransferR(Matrix::Row_Col_t rIdx1, Matrix::Row_Col_t rIdx2, Matrix::RealVal_t multiple)
{
    _ASSERT(rIdx1 >= 0 && rIdx1 < _Rows);
    _ASSERT(rIdx2 >= 0 && rIdx2 < _Rows);

    if (rIdx1 != rIdx2) {
        Row_Col_t r1 = rIdx1*_Cols;
        Row_Col_t r1_end = r1 + _Cols;
        Row_Col_t r2 = rIdx2*_Cols;

        if (DOUBLE_NEQZ(multiple)) {
            // Row-addition transformations (rIdx1 =>(+) rIdx2)
            for (; r1_end != r1; ++r1, ++r2) {
                if (DOUBLE_NEQZ(_ValueVec[r1]))
                    _ValueVec[r2] += _ValueVec[r1] * multiple;
            }
        }
        else {
            // Row-switching transformations (rIdx1 <=> rIdx2)
            RealVal_t tempVal = 0.0;

            for (; r1_end != r1; ++r1, ++r2) {
                tempVal = _ValueVec[r1];
                _ValueVec[r1] = _ValueVec[r2];
                _ValueVec[r2] = tempVal;
            }
        }
    }
    else {
        // Row-multiplying transformations
        _ASSERT(DOUBLE_NEQZ(multiple));

        Row_Col_t r = rIdx1*_Cols;
        Row_Col_t r_end = r + _Cols;

        for (; r_end != r; ++r)
            _ValueVec[r] *= multiple;
    }
}

void Matrix::ElementaryTransferC(Matrix::Row_Col_t cIdx1, Matrix::Row_Col_t cIdx2, Matrix::RealVal_t multiple)
{
    _ASSERT(cIdx1 >= 0 && cIdx1 < _Cols);
    _ASSERT(cIdx2 >= 0 && cIdx2 < _Cols);

    if (cIdx1 != cIdx2) {
        Row_Col_t c1 = cIdx1;
        Row_Col_t c1_end = c1 + _ValueVec.size();
        Row_Col_t c2 = cIdx2;
        if (DOUBLE_NEQZ(multiple)) {
            // Col-addition transformations (cIdx1 =>(+) cIdx2)
            for (; c1 != c1_end; c1 += _Cols, c2 += _Cols) {
                if (DOUBLE_NEQZ(_ValueVec[c1]))
                    _ValueVec[c2] += _ValueVec[c1] * multiple;
            }
        }
        else {
            // Col-switching transformations (cIdx1 <=> cIdx2)
            RealVal_t tempVal = 0.0;

            for (; c1 != c1_end; c1 += _Cols, c2 += _Cols) {
                tempVal = _ValueVec[c1];
                _ValueVec[c1] = _ValueVec[c2];
                _ValueVec[c2] = tempVal;
            }
        }
    }
    else {
        // Col-multiplying transformations
        _ASSERT(DOUBLE_NEQZ(multiple));

        Row_Col_t c = cIdx1;
        Row_Col_t c_end = c + _ValueVec.size();

        for (; c != c_end; c += _Cols)
            _ValueVec[c] *= multiple;
    }
}

string Matrix::ToString() const
{
    vector<ostream::pos_type> posVec;
    ostringstream strStream;
    string::size_type maxStrLen = 0;

    strStream << ' ';
    posVec.push_back(strStream.tellp());
    for (RealValCIter_t iter = _ValueVec.begin(); iter != _ValueVec.end(); ++iter)
    {
        ostringstream::pos_type sPos = strStream.tellp();

        strStream << (DOUBLE_EQZ(*iter) ? fabs(*iter) : *iter); // avoid '-0' appears in the result
        posVec.push_back(strStream.tellp());

        string::size_type curStrLen = strStream.tellp() - sPos;

        maxStrLen = (curStrLen > maxStrLen ? curStrLen : maxStrLen); // Get max double string length
    }

    string spaceStr(maxStrLen, ' ');
    string valStr(strStream.str());

    strStream.str("");
    for (Row_Col_t r = 0; r < _Rows; ++r)
    {
        Row_Col_t rSIdx = r * _Cols;
        for (Row_Col_t c = 0; c < _Cols; ++c)
        {
            ostream::pos_type strSIdx = posVec[rSIdx + c];
            ostream::pos_type strEIdx = posVec[rSIdx + c + 1];

            strStream.put(' ');
            strStream.write(spaceStr.c_str(), (maxStrLen - (strEIdx - strSIdx)));
            strStream.write(valStr.c_str() + strSIdx, (strEIdx - strSIdx));
        }
        if (r + 1 < _Rows)
            strStream.put('\n');
    }

    return (strStream.str());
}
