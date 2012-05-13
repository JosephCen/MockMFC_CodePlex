#ifndef ParameterSet_H
#define ParameterSet_H

#include <string>
#include "Matrix.h"
#include <crtdbg.h>

enum ParamTypeEnum
{
    PT_String,
    PT_RealVal,
    PT_Integer,
    PT_Matrix,
};

class Parameter
{
private :
    ParamTypeEnum _Type;
    union
    {
        int _Int;
        Matrix::RealVal_t _RealVal;
        std::string *_pStr;
        Matrix *_pMatrix;
    };
public :
    // Copy Constructor
    Parameter(const Parameter &param);
    // Constructors
    Parameter(): _Type(PT_Integer), _Int(0) { }
    explicit Parameter(int intVal): _Type(PT_Integer), _Int(intVal) { }
    explicit Parameter(Matrix::RealVal_t realVal): _Type(PT_RealVal), _RealVal(realVal) { }
    explicit Parameter(std::string &strVal): _Type(PT_String), _pStr(new std::string(strVal)) { }
    explicit Parameter(Matrix &matrixVal): _Type(PT_Matrix), _pMatrix(new Matrix(matrixVal)) { }
    explicit Parameter(Matrix *pMatrix): _Type(PT_Matrix), _pMatrix(pMatrix) { }
    // Destructor
    ~Parameter();
    // Methods
    bool IsInteger() const;
    int& IntegerVal();
    bool IsRealVal() const;
    Matrix::RealVal_t& RealVal();
    bool IsString() const;
    std::string& StringVal();
    bool IsMatrix() const;
    Matrix& MatrixVal();
    // Override Operators
    Parameter& operator=(const Parameter &param);
};

inline bool Parameter::IsInteger() const
{
    return PT_Integer == _Type;
}

inline int& Parameter::IntegerVal()
{
    _ASSERT(IsInteger());

    return _Int;
}

inline bool Parameter::IsRealVal() const
{
    return PT_RealVal == _Type;
}

inline Matrix::RealVal_t& Parameter::RealVal()
{
    _ASSERT(IsRealVal());

    return _RealVal;
}

inline bool Parameter::IsString() const
{
    return PT_String == _Type;
}

inline std::string& Parameter::StringVal()
{
    _ASSERT(IsString());

    return *_pStr;
}

inline bool Parameter::IsMatrix() const
{
    return PT_Matrix == _Type;
}

inline Matrix& Parameter::MatrixVal()
{
    _ASSERT(IsMatrix());

    return *_pMatrix;
}

#endif