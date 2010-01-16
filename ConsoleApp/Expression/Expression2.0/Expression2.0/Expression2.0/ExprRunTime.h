#ifndef ExprRunTime_H
#define ExprRunTime_H

#include "Matrix.h"
#include "ParameterSet.h"
#include <vector>
#include <stack>
#include <string>
#include <crtdbg.h>

enum ExprILCodeEnum
{
    EIL_NUL = -1,
    EIL_PushInteger,
    EIL_PushRealVal,
    EIL_RealValPlus,
    EIL_RealValMinus,
    EIL_RealValMultiply,
    EIL_RealValDivide,
    EIL_CtorMatrix,
    EIL_MatrixPlus,
    EIL_MatrixMinus,
    EIL_MatrixMultiply,
    EIL_MatrixDotMultiply,
    EIL_MatrixDivide,
    EIL_MatrixDotDivide,
    EIL_MatrixValPlus,
    EIL_MatrixValMinus,
    EIL_MatrixValMultiply,
    EIL_MatrixValDivide,
    EIL_Count,
};

class ExprILUnit
{
private :
    ExprILCodeEnum _Code;
    union
    {
        int _Int;
        Matrix::RealVal_t _RealVal;
        Matrix::RowCol_t _RowColPair;
    };
public :
    // Constructors
    ExprILUnit(): _Code(EIL_NUL), _Int(0) { }
    ExprILUnit(ExprILCodeEnum code): _Code(code), _Int(0) { _ASSERT(EIL_PushInteger != _Code && EIL_PushRealVal != _Code); }
    ExprILUnit(ExprILCodeEnum code, int intVal): _Code(code), _Int(intVal) { _ASSERT(EIL_PushInteger == _Code); }
    ExprILUnit(ExprILCodeEnum code, Matrix::RealVal_t realVal): _Code(code), _RealVal(realVal) { _ASSERT(EIL_PushRealVal == _Code); }
    ExprILUnit(ExprILCodeEnum code, Matrix::Row_Col_t rows, Matrix::Row_Col_t cols): _Code(code) { _ASSERT(EIL_CtorMatrix == _Code); _RowColPair._Rows = rows; _RowColPair._Cols = cols; }
    // Methods
    ExprILCodeEnum Code() const { return _Code; }
    int IntegerVal() const { _ASSERT(EIL_PushInteger == _Code); return _Int; }
    Matrix::RealVal_t RealVal() const { _ASSERT(EIL_PushRealVal == _Code); return _RealVal; }
    Matrix::RowCol_t RowColVal() const { _ASSERT(EIL_CtorMatrix == _Code); return _RowColPair; }
    std::string ToString() const;
};

class ExprILSegment
{
public :
    typedef std::vector<ExprILUnit> ILUnitVec_t;
    typedef ILUnitVec_t::size_type Len_t;
    typedef ILUnitVec_t::const_iterator ILRunPtr_t;
private :
    ILUnitVec_t _ILUnitVec;
public :
    // Constructor
    ExprILSegment(): _ILUnitVec() { }
    // Methods
    ILRunPtr_t StartRunPrt() const { return _ILUnitVec.begin(); }
    ILRunPtr_t EndRunPrt() const { return _ILUnitVec.end(); }
    Len_t Length() const { return _ILUnitVec.size(); }
    void Append(const ExprILUnit &ilUnit) { _ILUnitVec.push_back(ilUnit); }
    void Append(const ExprILSegment &ilSegment) { _ILUnitVec.insert(_ILUnitVec.end(), ilSegment.StartRunPrt(), ilSegment.EndRunPrt()); }
    std::string ToString() const;
};

enum ResultTypeEnum
{
    RT_None,
    RT_String,
    RT_Matrix,
    RT_RealVal
};

class ExprRunTime
{
private :
    typedef bool(ExprRunTime::*pFuncExprILUnit_t)(const ExprILUnit &ilUnit);

    static pFuncExprILUnit_t *s_pFuncExprILUnitArr;
    static bool s_IsInitialized;

    const ExprILSegment &_ILSegment;
    std::stack<Parameter, std::vector<Parameter> > _RunStack;
public :
    ExprRunTime(const ExprILSegment &ilSegment): _ILSegment(ilSegment), _RunStack() { }
    bool Run();
private :
    static void Initialize();
    static pFuncExprILUnit_t* GetFuncExprILUnitArr();
private :
    bool PushInteger(const ExprILUnit &ilUnit);
    bool PushRealVal(const ExprILUnit &ilUnit);
    bool RealValPlus(const ExprILUnit &ilUnit);
    bool RealValMinus(const ExprILUnit &ilUnit);
    bool RealValMultiply(const ExprILUnit &ilUnit);
    bool RealValDivide(const ExprILUnit &ilUnit);
    bool CtorMatrix(const ExprILUnit &ilUnit);
    bool MatrixPlus(const ExprILUnit &ilUnit);
    bool MatrixMinus(const ExprILUnit &ilUnit);
    bool MatrixMultiply(const ExprILUnit &ilUnit);
    bool MatrixDotMultiply(const ExprILUnit &ilUnit);
    bool MatrixDivide(const ExprILUnit &ilUnit);
    bool MatrixDotDivide(const ExprILUnit &ilUnit);
    bool MatrixValPlus(const ExprILUnit &ilUnit);
    bool MatrixValMinus(const ExprILUnit &ilUnit);
    bool MatrixValMultiply(const ExprILUnit &ilUnit);
    bool MatrixValDivide(const ExprILUnit &ilUnit);
};

inline ExprRunTime::pFuncExprILUnit_t* ExprRunTime::GetFuncExprILUnitArr()
{
    if (!s_IsInitialized)
        Initialize();

    return s_pFuncExprILUnitArr;
}

#endif