#ifndef Matrix_H
#define Matrix_H

#include <vector>
#include <string>

//---------------------------------------------------------------------
// Matrix - class
//---------------------------------------------------------------------
class Matrix
{
public :
    typedef double RealVal_t;
    typedef std::vector<RealVal_t>::size_type Row_Col_t;
    typedef std::vector<RealVal_t> RealValVec_t;
    typedef std::vector<RealVal_t>::iterator RealValIter_t;
    typedef std::vector<RealVal_t>::const_iterator RealValCIter_t;
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
    Matrix(Row_Col_t rows, Row_Col_t cols, char signCh);
	Matrix(Row_Col_t rows, Row_Col_t cols, RealVal_t *realValArr);
    Matrix(RowCol_t rowcol, RealValVec_t &realValVecRef);
    // Methods
    Row_Col_t Rows() const;
    Row_Col_t Cols() const;
    Matrix& DotAssignmentMultiply(const Matrix &matrix);
    Matrix& DotAssignmentDivid(const Matrix &matrix);
    std::string ToString() const;
    bool IsSingleValue() const;
    RealVal_t At(Row_Col_t rows, Row_Col_t cols) const;
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
    Matrix& operator/=(const Matrix &matrix);
    Matrix& ToInversion();
private :
    // Methods
    bool InversionTransToR(Matrix *pInvMatrix, std::string *pErrStr);
    bool InversionTransToC(Matrix *pInvMatrix, std::string *pErrStr);
    bool MultiplyInto(const Matrix &matrixL, const Matrix &matrixR, std::string *pErrStr);
    void ElementaryTransferR(Row_Col_t rIdx1, Row_Col_t rIdx2, RealVal_t multiple);
    void ElementaryTransferC(Row_Col_t cIdx1, Row_Col_t cIdx2, RealVal_t multiple);
    // Friend Class Declaration
    friend Matrix operator*(const Matrix &matrix1, const Matrix &matrix2);
};

Matrix operator+(const Matrix &matrix1, const Matrix &matrix2);
Matrix operator-(const Matrix &matrix1, const Matrix &matrix2);
Matrix operator*(const Matrix &matrix1, const Matrix &matrix2);

inline Matrix::Row_Col_t Matrix::Rows() const
{
    return (_Rows);
}

inline Matrix::Row_Col_t Matrix::Cols() const
{
    return (_Cols);
}

inline Matrix& Matrix::DotAssignmentMultiply(const Matrix &matrix)
{
    return (this->DotAssignmentOperator(matrix, '*'));
}

inline Matrix& Matrix::DotAssignmentDivid(const Matrix &matrix)
{
    return (this->DotAssignmentOperator(matrix, '/'));
}

inline bool Matrix::IsSingleValue() const
{
    return (1 == _Rows && 1 == _Cols);
}

inline Matrix::RealVal_t Matrix::At(Matrix::Row_Col_t rows, Matrix::Row_Col_t cols) const
{
    return (_ValueVec[rows * _Cols + cols]);
}

#endif