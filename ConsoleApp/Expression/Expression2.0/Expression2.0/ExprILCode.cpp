#include "stdafx.h"
#include "ExprILCode.h"
#include "ExprException.h"
#include "DoubleComparison.h"
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <crtdbg.h>

//---------------------------------------------------------------------
// Class member - ExprILRunState
//---------------------------------------------------------------------
ExprILRunState::ExprILRunState(VariableSet *pVarSet, bool isNewVarSet) : 
_IsNewVarSet(isNewVarSet), _pVarSet(NULL), _VarStack()
{
    _ASSERT(NULL != pVarSet);

    if (_IsNewVarSet)
        _pVarSet = new VariableSet(pVarSet, false);
    else
        _pVarSet = pVarSet;
}

ExprILRunState::~ExprILRunState()
{
    if (_IsNewVarSet) {
        delete _pVarSet;
        _pVarSet = NULL;
    }
}

//---------------------------------------------------------------------
// Class member - ExprILCode
//---------------------------------------------------------------------
ExprILCode::ExprILCode()
{ }

ExprILCode::~ExprILCode()
{ }

string ExprILCode::ToString() const
{
    ostringstream oStrStream;

    ToString(&oStrStream);

    return oStrStream.str();
}

//---------------------------------------------------------------------
// Class member - PushIntegerILCode
//---------------------------------------------------------------------
PushIntegerILCode::PushIntegerILCode(int intValue) :
_IntValue(intValue)
{ }

ExprILCodeEnum PushIntegerILCode::GetCodeEnum() const
{
    return EIL_PushInteger;
}

bool PushIntegerILCode::RunCode(ExprILRunState *pILRunState)
{
    IntVariable *pIntVariable = NULL;

    pIntVariable = new IntVariable(_IntValue);
    GetVariableStack(pILRunState)->PushVar(pIntVariable);

    return true;
}

void PushIntegerILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "PushInteger " << _IntValue;
}

//---------------------------------------------------------------------
// Class member - PushRealValILCode
//---------------------------------------------------------------------
PushRealValILCode::PushRealValILCode(Matrix::RealVal_t realValue) :
_RealValue(realValue)
{ }

ExprILCodeEnum PushRealValILCode::GetCodeEnum() const
{
    return EIL_PushRealVal;
}

bool PushRealValILCode::RunCode(ExprILRunState *pILRunState)
{
    RealVariable *pRealVariable = NULL;

    pRealVariable = new RealVariable(_RealValue);
    GetVariableStack(pILRunState)->PushVar(pRealVariable);

    return true;
}

void PushRealValILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "PushRealVal " << _RealValue;
}

//---------------------------------------------------------------------
// Class member - RealValBinaryOperILCode
//---------------------------------------------------------------------
RealValBinaryOperILCode::RealValBinaryOperILCode()
{ }

bool RealValBinaryOperILCode::RunCode(ExprILRunState *pILRunState)
{
    _ASSERT(GetVariableStack(pILRunState)->Count() >= 2);

    Variable *pVariableR = NULL;
    Variable *pVariableL = NULL;

    if (RealVariable::TypeId == GetVariableStack(pILRunState)->TopVar(&pVariableR)
        && RealVariable::TypeId == GetVariableStack(pILRunState)->TopVar(&pVariableL, 1)) {
            if (DoOperator((RealVariable*)pVariableL, (RealVariable*)pVariableR)) {
                GetVariableStack(pILRunState)->RemoveTopVar();

                return true;
            }
    }
    
    return false;
}

//---------------------------------------------------------------------
// Class member - RealValPlusILCode
//---------------------------------------------------------------------
RealValPlusILCode::RealValPlusILCode()
{ }

ExprILCodeEnum RealValPlusILCode::GetCodeEnum() const
{
    return EIL_RealValPlus;
}

bool RealValPlusILCode::DoOperator(RealVariable *pVariableL, RealVariable *pVariableR)
{
    pVariableL->GetValueRef() += pVariableR->GetValue();

    return true;
}

void RealValPlusILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "RealValPlus";
}

//---------------------------------------------------------------------
// Class member - RealValMinusILCode
//---------------------------------------------------------------------
RealValMinusILCode::RealValMinusILCode()
{ }

ExprILCodeEnum RealValMinusILCode::GetCodeEnum() const
{
    return EIL_RealValMinus;
}

bool RealValMinusILCode::DoOperator(RealVariable *pVariableL, RealVariable *pVariableR)
{
    pVariableL->GetValueRef() -= pVariableR->GetValue();

    return true;
}

void RealValMinusILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "RealValMinus";
}

//---------------------------------------------------------------------
// Class member - RealValMultiplyILCode
//---------------------------------------------------------------------
RealValMultiplyILCode::RealValMultiplyILCode()
{ }

ExprILCodeEnum RealValMultiplyILCode::GetCodeEnum() const
{
    return EIL_RealValMultiply;
}

bool RealValMultiplyILCode::DoOperator(RealVariable *pVariableL, RealVariable *pVariableR)
{
    pVariableL->GetValueRef() *= pVariableR->GetValue();

    return true;
}

void RealValMultiplyILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "RealValMultiply";
}

//---------------------------------------------------------------------
// Class member - RealValDivideILCode
//---------------------------------------------------------------------
RealValDivideILCode::RealValDivideILCode()
{ }

ExprILCodeEnum RealValDivideILCode::GetCodeEnum() const
{
    return EIL_RealValDivide;
}

bool RealValDivideILCode::DoOperator(RealVariable *pVariableL, RealVariable *pVariableR)
{
    if (DOUBLE_NEQZ(pVariableR->GetValue())) {
        pVariableL->GetValueRef() /= pVariableR->GetValue();

        return true;
    }

    return false;
}

void RealValDivideILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "RealValDivide";
}

//---------------------------------------------------------------------
// Class member - CtorMatrixILCode
//---------------------------------------------------------------------
CtorMatrixILCode::CtorMatrixILCode(Matrix::Row_Col_t rows, Matrix::Row_Col_t cols)
{
    _ASSERT(rows > 0 && cols > 0);

    _RowColPair._Rows = rows;
    _RowColPair._Cols = cols;
}

ExprILCodeEnum CtorMatrixILCode::GetCodeEnum() const
{
    return EIL_CtorMatrix;
}

bool CtorMatrixILCode::RunCode(ExprILRunState *pILRunState)
{
    _ASSERT(GetVariableStack(pILRunState)->Count() >= static_cast<int>(_RowColPair._Rows * _RowColPair._Cols));

    Matrix::RealValVec_t realValVec((_RowColPair._Rows * _RowColPair._Cols), 0.0);
    Matrix::RealValVec_t::iterator iter = realValVec.end();
    int vecLen = static_cast<int>(realValVec.size());
    Variable *pVariable = NULL;
    bool state = true;

    for (int i = 0; i < vecLen; ++i) {
        if (RealVariable::TypeId == GetVariableStack(pILRunState)->TopVar(&pVariable, i)) {
            --iter;
            *iter = ((RealVariable*)pVariable)->GetValue();
        }
        else {
            state = false;
            break;
        }
    }

    MatrixVariable *pMatrixVar = NULL;

    if (state) {
        GetVariableStack(pILRunState)->RemoveTopVar(vecLen);
        pMatrixVar = new MatrixVariable();
        pMatrixVar->GetValueRef().Swap(_RowColPair, realValVec);
        GetVariableStack(pILRunState)->PushVar(pMatrixVar);
    }

    return state;
}

void CtorMatrixILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "CtorMatrix [" << _RowColPair._Rows << ',' << _RowColPair._Cols << ']';
}

//---------------------------------------------------------------------
// Class member - MatrixBinaryOperILCode
//---------------------------------------------------------------------
MatrixBinaryOperILCode::MatrixBinaryOperILCode()
{ }

bool MatrixBinaryOperILCode::RunCode(ExprILRunState *pILRunState)
{
    _ASSERT(GetVariableStack(pILRunState)->Count() >= 2);

    Variable *pVariableR = NULL;
    Variable *pVariableL = NULL;

    if (MatrixVariable::TypeId == GetVariableStack(pILRunState)->TopVar(&pVariableR)
        && MatrixVariable::TypeId == GetVariableStack(pILRunState)->TopVar(&pVariableL, 1)) {
            if (DoOperator((MatrixVariable*)pVariableL, (MatrixVariable*)pVariableR)) {
                GetVariableStack(pILRunState)->RemoveTopVar();

                return true;
            }
    }
    
    return false;
}

//---------------------------------------------------------------------
// Class member - MatrixPlusILCode
//---------------------------------------------------------------------
MatrixPlusILCode::MatrixPlusILCode()
{ }

ExprILCodeEnum MatrixPlusILCode::GetCodeEnum() const
{
    return EIL_MatrixPlus;
}

bool MatrixPlusILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR)
{
    bool state = true;

    try {
        pVariableL->GetValueRef() += pVariableR->GetValueRef();
    }
    catch (ExprException&) {
        state = false;
    }

    return state;
}

void MatrixPlusILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "MatrixPlus";
}

//---------------------------------------------------------------------
// Class member - MatrixMinusILCode
//---------------------------------------------------------------------
MatrixMinusILCode::MatrixMinusILCode()
{ }

ExprILCodeEnum MatrixMinusILCode::GetCodeEnum() const
{
    return EIL_MatrixMinus;
}

bool MatrixMinusILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR)
{
    bool state = true;

    try {
        pVariableL->GetValueRef() -= pVariableR->GetValueRef();
    }
    catch (ExprException&) {
        state = false;
    }

    return state;
}

void MatrixMinusILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "MatrixMinus";
}

//---------------------------------------------------------------------
// Class member - MatrixDotMultiplyILCode
//---------------------------------------------------------------------
MatrixDotMultiplyILCode::MatrixDotMultiplyILCode()
{ }

ExprILCodeEnum MatrixDotMultiplyILCode::GetCodeEnum() const
{
    return EIL_MatrixDotMultiply;
}

bool MatrixDotMultiplyILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR)
{
    bool state = true;

    try {
        pVariableL->GetValueRef().DotAssignmentMultiply(pVariableR->GetValueRef());
    }
    catch (ExprException&) {
        state = false;
    }

    return state;
}

void MatrixDotMultiplyILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "MatrixDotMultiply";
}

//---------------------------------------------------------------------
// Class member - MatrixDotDivideILCode
//---------------------------------------------------------------------
MatrixDotDivideILCode::MatrixDotDivideILCode()
{ }

ExprILCodeEnum MatrixDotDivideILCode::GetCodeEnum() const
{
    return EIL_MatrixDotDivide;
}

bool MatrixDotDivideILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR)
{
    bool state = true;

    try {
        pVariableL->GetValueRef().DotAssignmentDivid(pVariableR->GetValueRef());
    }
    catch (ExprException&) {
        state = false;
    }

    return state;
}

void MatrixDotDivideILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "MatrixDotDivide";
}

//---------------------------------------------------------------------
// Class member - MatrixValBinaryOperILCode
//---------------------------------------------------------------------
MatrixValBinaryOperILCode::MatrixValBinaryOperILCode()
{ }

bool MatrixValBinaryOperILCode::RunCode(ExprILRunState *pILRunState)
{
    _ASSERT(GetVariableStack(pILRunState)->Count() >= 2);

    Variable *pVariableR = NULL;
    Variable *pVariableL = NULL;

    if (MatrixVariable::TypeId == GetVariableStack(pILRunState)->TopVar(&pVariableR)
        && RealVariable::TypeId == GetVariableStack(pILRunState)->TopVar(&pVariableL, 1)) {
            if (DoOperator((MatrixVariable*)pVariableL, (RealVariable*)pVariableR)) {
                GetVariableStack(pILRunState)->RemoveTopVar();

                return true;
            }
    }

    return false;
}

//---------------------------------------------------------------------
// Class member - MatrixValPlusILCode
//---------------------------------------------------------------------
MatrixValPlusILCode::MatrixValPlusILCode()
{ }

ExprILCodeEnum MatrixValPlusILCode::GetCodeEnum() const
{
    return EIL_MatrixValPlus;
}

bool MatrixValPlusILCode::DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR)
{
    pVariableL->GetValueRef() += pVariableR->GetValue();

    return true;
}

void MatrixValPlusILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "MatrixValPlus";
}

//---------------------------------------------------------------------
// Class member - MatrixValMinusILCode
//---------------------------------------------------------------------
MatrixValMinusILCode::MatrixValMinusILCode()
{ }

ExprILCodeEnum MatrixValMinusILCode::GetCodeEnum() const
{
    return EIL_MatrixValMinus;
}

bool MatrixValMinusILCode::DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR)
{
    pVariableL->GetValueRef() -= pVariableR->GetValue();

    return true;
}

void MatrixValMinusILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "MatrixValMinus";
}

//---------------------------------------------------------------------
// Class member - MatrixValMultiplyILCode
//---------------------------------------------------------------------
MatrixValMultiplyILCode::MatrixValMultiplyILCode()
{ }

ExprILCodeEnum MatrixValMultiplyILCode::GetCodeEnum() const
{
    return EIL_MatrixValMultiply;
}

bool MatrixValMultiplyILCode::DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR)
{
    pVariableL->GetValueRef() *= pVariableR->GetValue();

    return true;
}

void MatrixValMultiplyILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "MatrixValMultiply";
}

//---------------------------------------------------------------------
// Class member - MatrixValDivideILCode
//---------------------------------------------------------------------
MatrixValDivideILCode::MatrixValDivideILCode()
{ }

ExprILCodeEnum MatrixValDivideILCode::GetCodeEnum() const
{
    return EIL_MatrixValDivide;
}

bool MatrixValDivideILCode::DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR)
{
    bool state = true;

    try {
        pVariableL->GetValueRef() /= pVariableR->GetValue();
    }
    catch (ExprException&) {
        state = false;
    }

    return state;
}

void MatrixValDivideILCode::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "MatrixValDivide";
}