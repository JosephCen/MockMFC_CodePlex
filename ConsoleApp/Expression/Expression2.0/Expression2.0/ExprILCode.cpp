#include "stdafx.h"
#include "ExprILCode.h"
#include <vector>
using std::vector;
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
    if (_IsNewVarSet)
        delete _pVarSet;
}

//---------------------------------------------------------------------
// Class member - ExprILCode
//---------------------------------------------------------------------
ExprILCode::ExprILCode() :
_pILRunState(NULL)
{ }

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

bool PushIntegerILCode::RunCode()
{
    IntVariable *pIntVariable = NULL;

    pIntVariable = new IntVariable(_IntValue);
    GetVariableStack()->PushVar(pIntVariable);

    return true;
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

bool PushRealValILCode::RunCode()
{
    RealVariable *pRealVariable = NULL;

    pRealVariable = new RealVariable(_RealValue);
    GetVariableStack()->PushVar(pRealVariable);

    return true;
}

//---------------------------------------------------------------------
// Class member - RealValBinaryOperILCode
//---------------------------------------------------------------------
RealValBinaryOperILCode::RealValBinaryOperILCode()
{ }

bool RealValBinaryOperILCode::RunCode()
{
    _ASSERT(GetVariableStack()->Count() >= 2);

    Variable *pVariableR = NULL;
    Variable *pVariableL = NULL;

    if (RealVariable::TypeId == GetVariableStack()->TopVar(&pVariableR)
        && RealVariable::TypeId == GetVariableStack()->TopVar(&pVariableL, 1)) {
            if (DoOperator((RealVariable*)pVariableL, (RealVariable*)pVariableR)) {
                GetVariableStack()->RemoveTopVar();

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
    if (0 != pVariableR->GetValue()) {
        pVariableL->GetValueRef() /= pVariableR->GetValue();

        return true;
    }

    return false;
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

bool CtorMatrixILCode::RunCode()
{
    _ASSERT(GetVariableStack()->Count() >= (_RowColPair._Rows * _RowColPair._Cols));

    Matrix::RealValVec_t realValVec((_RowColPair._Rows * _RowColPair._Cols), 0.0);
    Matrix::RealValVec_t::iterator iter = realValVec.end();
    int vecLen = realValVec.size();
    Variable *pVariable = NULL;
    bool state = true;

    for (int i = 0; i < vecLen; ++i) {
        if (RealVariable::TypeId == GetVariableStack()->TopVar(&pVariable, i)) {
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
        GetVariableStack()->RemoveTopVar(vecLen);
        pMatrixVar = new MatrixVariable();
        pMatrixVar->GetValueRef().Swap(_RowColPair, realValVec);
        GetVariableStack()->PushVar(pMatrixVar);
    }

    return state;
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

bool MatrixPlusILCode::RunCode()
{
    // TODO
    return true;
}