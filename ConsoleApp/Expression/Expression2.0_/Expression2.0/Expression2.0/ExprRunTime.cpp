#include "stdafx.h"
#include "ExprRunTime.h"
#include "ParameterSet.h"
#include <vector>
using std::vector;
#include <math.h>
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <crtdbg.h>

const Matrix::RealVal_t g_MathError = 0.000001;

// Member of 'ExprILUnit'

string ExprILUnit::ToString() const
{
    ostringstream resultStrS;

    switch (_Code) {
        case EIL_NUL :
            resultStrS << "NUL";
            break;
        case EIL_PushInteger :
            resultStrS << "PushInteger " << _Int;
            break;
        case EIL_PushRealVal :
            resultStrS << "PushRealVal " << _RealVal;
            break;
        case EIL_RealValPlus :
            resultStrS << "RealValPlus ";
            break;
        case EIL_RealValMinus :
            resultStrS << "RealValMinus";
            break;
        case EIL_RealValMultiply :
            resultStrS << "RealValMultiply";
            break;
        case EIL_RealValDivide :
            resultStrS << "RealValDivide";
            break;
        case EIL_CtorMatrix :
            resultStrS << "CtorMatrix " << _RowColPair._Rows << "," << _RowColPair._Cols;
            break;
        case EIL_MatrixPlus :
            resultStrS << "MatrixPlus";
            break;
        case EIL_MatrixMinus :
            resultStrS << "MatrixMinus";
            break;
        case EIL_MatrixMultiply :
            resultStrS << "MatrixMultiply";
            break;
        case EIL_MatrixDotMultiply :
            resultStrS << "MatrixDotMultiply";
            break;
        case EIL_MatrixDivide :
            resultStrS << "MatrixDivide";
            break;
        case EIL_MatrixDotDivide :
            resultStrS << "MatrixDotDivide";
            break;
    }

    return resultStrS.str();
}

// Member of 'ExprILSegment'

string ExprILSegment::ToString() const
{
    ostringstream resultStrS;

    resultStrS << "Code[" << Length() << "] =";

    for (ILRunPtr_t codePtr = StartRunPrt(); codePtr != EndRunPrt(); ++codePtr) {
        resultStrS << "\n" << codePtr->ToString();
    }

    return resultStrS.str();
}

// Member of 'ExprRunTime'

bool ExprRunTime::s_IsInitialized = false;

ExprRunTime::pFuncExprILUnit_t *ExprRunTime::s_pFuncExprILUnitArr = NULL;

void ExprRunTime::Initialize()
{
    if (!s_IsInitialized) {
        s_pFuncExprILUnitArr = new pFuncExprILUnit_t[EIL_Count];
        s_pFuncExprILUnitArr[EIL_PushInteger]       = &ExprRunTime::PushInteger;
        s_pFuncExprILUnitArr[EIL_PushRealVal]       = &ExprRunTime::PushRealVal;
        s_pFuncExprILUnitArr[EIL_RealValPlus]       = &ExprRunTime::RealValPlus;
        s_pFuncExprILUnitArr[EIL_RealValMinus]      = &ExprRunTime::RealValMinus;
        s_pFuncExprILUnitArr[EIL_RealValMultiply]   = &ExprRunTime::RealValMultiply;
        s_pFuncExprILUnitArr[EIL_RealValDivide]     = &ExprRunTime::RealValDivide;
        s_pFuncExprILUnitArr[EIL_CtorMatrix]        = &ExprRunTime::CtorMatrix;
        s_pFuncExprILUnitArr[EIL_MatrixPlus]        = &ExprRunTime::MatrixPlus;
        s_pFuncExprILUnitArr[EIL_MatrixMinus]       = &ExprRunTime::MatrixMinus;
        s_pFuncExprILUnitArr[EIL_MatrixMultiply]    = &ExprRunTime::MatrixMultiply;
        s_pFuncExprILUnitArr[EIL_MatrixDotMultiply] = &ExprRunTime::MatrixDotMultiply;
        s_pFuncExprILUnitArr[EIL_MatrixDivide]      = &ExprRunTime::MatrixDivide;
        s_pFuncExprILUnitArr[EIL_MatrixDotDivide]   = &ExprRunTime::MatrixDotDivide;
        s_pFuncExprILUnitArr[EIL_MatrixValPlus]     = &ExprRunTime::MatrixValPlus;
        s_pFuncExprILUnitArr[EIL_MatrixValMinus]    = &ExprRunTime::MatrixValMinus;
        s_pFuncExprILUnitArr[EIL_MatrixValMultiply] = &ExprRunTime::MatrixValMultiply;
        s_pFuncExprILUnitArr[EIL_MatrixValDivide]   = &ExprRunTime::MatrixValDivide;

        s_IsInitialized = true;
    }
}

bool ExprRunTime::Run()
{
    ExprILSegment::ILRunPtr_t curRunPrt = _ILSegment.StartRunPrt();
    ExprILSegment::ILRunPtr_t endRunPrt = _ILSegment.EndRunPrt();

    bool isSuccess = true;

    while (endRunPrt != curRunPrt && isSuccess) {
        isSuccess = (this->*GetFuncExprILUnitArr()[curRunPrt->Code()])(*curRunPrt);
        ++curRunPrt;
    }

    return isSuccess;
}

bool ExprRunTime::PushInteger(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_PushInteger == ilUnit.Code());

    _RunStack.push(Parameter(ilUnit.RealVal()));

    return true;
}

bool ExprRunTime::PushRealVal(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_PushRealVal == ilUnit.Code());

    _RunStack.push(Parameter(ilUnit.RealVal()));

    return true;
}

bool ExprRunTime::RealValPlus(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_RealValPlus == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    Matrix::RealVal_t realVal = _RunStack.top().RealVal();
    _RunStack.pop();
    _RunStack.top().RealVal() += realVal;

    return true;
}

bool ExprRunTime::RealValMinus(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_RealValMinus == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    Matrix::RealVal_t realVal = _RunStack.top().RealVal();
    _RunStack.pop();
    _RunStack.top().RealVal() -= realVal;

    return true;
}

bool ExprRunTime::RealValMultiply(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_RealValMultiply == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    Matrix::RealVal_t realVal = _RunStack.top().RealVal();
    _RunStack.pop();
    _RunStack.top().RealVal() *= realVal;

    return true;
}

bool ExprRunTime::RealValDivide(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_RealValDivide == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    Matrix::RealVal_t realVal = _RunStack.top().RealVal();
    if (abs(realVal) >= g_MathError) {
        _RunStack.pop();
        _RunStack.top().RealVal() /= realVal;

        return true;
    }

    return false;
}

bool ExprRunTime::CtorMatrix(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_CtorMatrix == ilUnit.Code());

    Matrix::RowCol_t rowcol = ilUnit.RowColVal();
    Matrix::Row_Col_t realValNum = rowcol._Rows * rowcol._Cols;
    Matrix::RealValVec_t realValVec(realValNum, 0.0);

    _ASSERT(_RunStack.size() >= realValNum);

    Matrix::RealValVec_t::reverse_iterator rIter = realValVec.rbegin();
    Matrix::RealValVec_t::reverse_iterator rIterEnd = realValVec.rend();
    while (rIter != rIterEnd) {
        *rIter = _RunStack.top().RealVal();
        _RunStack.pop();
        ++rIter;
    }

    _RunStack.push(Parameter(new Matrix(rowcol, realValVec)));

    return true;
}

bool ExprRunTime::MatrixPlus(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixPlus == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    vector<Parameter> &runStackVec = _RunStack.c;
    Matrix &matrix_1 = runStackVec.at(runStackVec.size() - 2).MatrixVal();
    Matrix &matrix_2 = runStackVec.at(runStackVec.size() - 1).MatrixVal();

    matrix_1 += matrix_2;
    runStackVec.pop_back();

    return true;
}

bool ExprRunTime::MatrixMinus(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixMinus == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    vector<Parameter> &runStackVec = _RunStack.c;
    Matrix &matrix_1 = runStackVec.at(runStackVec.size() - 2).MatrixVal();
    Matrix &matrix_2 = runStackVec.at(runStackVec.size() - 1).MatrixVal();

    matrix_1 -= matrix_2;
    runStackVec.pop_back();

    return true;
}

bool ExprRunTime::MatrixMultiply(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixMultiply == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    vector<Parameter> &runStackVec = _RunStack.c;
    Matrix &matrix_1 = runStackVec.at(runStackVec.size() - 2).MatrixVal();
    Matrix &matrix_2 = runStackVec.at(runStackVec.size() - 1).MatrixVal();

    matrix_1 *= matrix_2;
    runStackVec.pop_back();

    return true;
}

bool ExprRunTime::MatrixDotMultiply(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixDotMultiply == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    vector<Parameter> &runStackVec = _RunStack.c;
    Matrix &matrix_1 = runStackVec.at(runStackVec.size() - 2).MatrixVal();
    Matrix &matrix_2 = runStackVec.at(runStackVec.size() - 1).MatrixVal();

    matrix_1.DotAssignmentMultiply(matrix_2);
    runStackVec.pop_back();

    return true;
}

bool ExprRunTime::MatrixDivide(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixDivide == ilUnit.Code());

    return false;
}

bool ExprRunTime::MatrixDotDivide(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixDotDivide == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    vector<Parameter> &runStackVec = _RunStack.c;
    Matrix &matrix_1 = runStackVec.at(runStackVec.size() - 2).MatrixVal();
    Matrix &matrix_2 = runStackVec.at(runStackVec.size() - 1).MatrixVal();

    matrix_1.DotAssignmentDivid(matrix_1);
    runStackVec.pop_back();

    return true;
}

bool ExprRunTime::MatrixValPlus(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixValPlus == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    Matrix::RealVal_t realVal = _RunStack.top().RealVal();
    _RunStack.pop();
    _RunStack.top().MatrixVal() += realVal;

    return true;
}

bool ExprRunTime::MatrixValMinus(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixValPlus == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    Matrix::RealVal_t realVal = _RunStack.top().RealVal();
    _RunStack.pop();
    _RunStack.top().MatrixVal() -= realVal;

    return true;
}

bool ExprRunTime::MatrixValMultiply(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixValMultiply == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    Matrix::RealVal_t realVal = _RunStack.top().RealVal();
    _RunStack.pop();
    _RunStack.top().MatrixVal() *= realVal;

    return true;
}

bool ExprRunTime::MatrixValDivide(const ExprILUnit &ilUnit)
{
    _ASSERT(EIL_MatrixValDivide == ilUnit.Code());
    _ASSERT(_RunStack.size() >= 2);

    Matrix::RealVal_t realVal = _RunStack.top().RealVal();
    _RunStack.pop();
    _RunStack.top().MatrixVal() /= realVal;

    return true;
}