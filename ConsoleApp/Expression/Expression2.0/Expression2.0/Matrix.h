#ifndef Matrix_H
#define Matrix_H

#include <vector>
#include <string>
#include <crtdbg.h>

//---------------------------------------------------------------------
// Matrix - class
//---------------------------------------------------------------------
class Matrix
{
public :
    typedef double RealVal_t;
    typedef std::vector<RealVal_t>::size_type Row_Col_t;
    typedef std::vector<RealVal_t> RealValVec_t;
    typedef struct 
    {
        Row_Col_t _Rows;
        Row_Col_t _Cols;
    } RowCol_t;
private :
	Row_Col_t _Rows;
	Row_Col_t _Cols;
	RealValVec_t _ValueVec;
private :
    Matrix& DotAssignmentOperator(const Matrix &matrix, char operCh);
    Matrix& DotAssignmentOperator(RealVal_t realVal, char operCh);
public :
	// Constructors
	Matrix();
	Matrix(Row_Col_t rows, Row_Col_t cols);
	Matrix(Row_Col_t rows, Row_Col_t cols, RealVal_t *realValArr);
    Matrix(RowCol_t rowcol, RealValVec_t &realValVecRef);
    // Methods
    Row_Col_t Rows() const { return _Rows; }
    Row_Col_t Cols() const { return _Cols; }
    Matrix& DotAssignmentMultiply(const Matrix &matrix) { return this->DotAssignmentOperator(matrix, '*'); }
    Matrix& DotAssignmentDivid(const Matrix &matrix) { return this->DotAssignmentOperator(matrix, '/'); }
    std::string ToString() const;
    bool IsSingleValue() const { return (1 == _Rows && 1 == _Cols); }
    RealVal_t At(Row_Col_t rows, Row_Col_t cols) const { _ASSERT(rows < _Rows && cols < _Cols); return _ValueVec[rows * _Cols + cols]; }
    void Swap(Matrix &matrix);
    void Swap(RowCol_t rowcol, RealValVec_t &realValVecRef);
    // Override Operators
    Matrix& operator+=(const Matrix &matrix) { return this->DotAssignmentOperator(matrix, '+'); }
    Matrix& operator+=(RealVal_t realVal) { return this->DotAssignmentOperator(realVal, '+'); }
    Matrix& operator-=(const Matrix &matrix) { return this->DotAssignmentOperator(matrix, '-'); }
    Matrix& operator-=(RealVal_t realVal) { return this->DotAssignmentOperator(realVal, '-'); }
    Matrix& operator*=(const Matrix &matrix);
    Matrix& operator*=(RealVal_t realVal) { return this->DotAssignmentOperator(realVal, '*'); }
    Matrix& operator/=(RealVal_t realVal) { return this->DotAssignmentOperator(realVal, '/'); }
};

Matrix operator+(const Matrix &matrix1, const Matrix &matrix2);
Matrix operator-(const Matrix &matrix1, const Matrix &matrix2);
Matrix operator*(const Matrix &matrix1, const Matrix &matrix2);

#endif