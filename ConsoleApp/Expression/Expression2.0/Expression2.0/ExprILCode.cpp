#include "stdafx.h"
#include "ExprILCode.h"
#include "ExprException.h"
#include "DoubleComparison.h"
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <sstream>
using std::ostream;
using std::ostringstream;
#include <crtdbg.h>

//---------------------------------------------------------------------
// Class member - ExprILRunState
//---------------------------------------------------------------------
ExprILRunState::ExprILRunState(VariableSet *pVarSet, bool isNewVarSet) : 
ExprErrHolder(), _IsNewVarSet(isNewVarSet), _pVarSet(NULL), _VarStack()
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

void PushIntegerILCode::ToString(ostream *pOStream) const
{
    *pOStream << "PushInteger " << _IntValue;
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

void PushRealValILCode::ToString(ostream *pOStream) const
{
    *pOStream << "PushRealVal " << _RealValue;
}

//---------------------------------------------------------------------
// PushDefValILCode - class
//---------------------------------------------------------------------
PushDefValILCode::PushDefValILCode(const string &defValName) :
_DefValName(defValName)
{ }

ExprILCodeEnum PushDefValILCode::GetCodeEnum() const
{
    return EIL_PushDefVal;
}

bool PushDefValILCode::RunCode(ExprILRunState *pILRunState)
{
    Variable *pVariable = NULL;

    pVariable = pILRunState->GetVariableSet()->SearchVar(_DefValName);
    if (NULL != pVariable) {
        pILRunState->GetVariableStack()->PushVar(pVariable->Duplicate());

        return true;
    }
    else {
        pILRunState->SetError("Defined variable cannot be found.");
    }

    return false;
}

void PushDefValILCode::ToString(std::ostream *pOStream) const
{
    *pOStream << "PushDefVal " << _DefValName;
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
            if (DoOperator((RealVariable*)pVariableL, (RealVariable*)pVariableR, pILRunState)) {
                GetVariableStack(pILRunState)->RemoveTopVar();

                return true;
            }
    }
    else
    {
        pILRunState->SetError("One or more operand is invalid!");
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

bool RealValPlusILCode::DoOperator(RealVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState)
{
    pVariableL->GetValueRef() += pVariableR->GetValue();

    return true;
}

void RealValPlusILCode::ToString(ostream *pOStream) const
{
    *pOStream << "RealValPlus";
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

bool RealValMinusILCode::DoOperator(RealVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState)
{
    pVariableL->GetValueRef() -= pVariableR->GetValue();

    return true;
}

void RealValMinusILCode::ToString(ostream *pOStream) const
{
    *pOStream << "RealValMinus";
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

bool RealValMultiplyILCode::DoOperator(RealVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState)
{
    pVariableL->GetValueRef() *= pVariableR->GetValue();

    return true;
}

void RealValMultiplyILCode::ToString(ostream *pOStream) const
{
    *pOStream << "RealValMultiply";
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

bool RealValDivideILCode::DoOperator(RealVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState)
{
    _ASSERT(NULL != pILRunState);

    if (DOUBLE_NEQZ(pVariableR->GetValue())) {
        pVariableL->GetValueRef() /= pVariableR->GetValue();

        return true;
    }
    else
    {
        pILRunState->SetError("The second operand of '/' (divide) is Zero.");
    }

    return false;
}

void RealValDivideILCode::ToString(ostream *pOStream) const
{
    *pOStream << "RealValDivide";
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
            pILRunState->SetError("One or more invalid variable is given in a matrix.");
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

void CtorMatrixILCode::ToString(ostream *pOStream) const
{
    *pOStream << "CtorMatrix [" << _RowColPair._Rows << ',' << _RowColPair._Cols << ']';
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

    GetVariableStack(pILRunState)->TopVar(&pVariableR);
    GetVariableStack(pILRunState)->TopVar(&pVariableL, 1);
    if (MatrixVariable::TypeId == pVariableR->GetTypeId() && MatrixVariable::TypeId == pVariableL->GetTypeId()) {
        if (DoOperator((MatrixVariable*)pVariableL, (MatrixVariable*)pVariableR, pILRunState)) {
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

bool MatrixPlusILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState)
{
    _ASSERT(NULL != pILRunState);

    bool state = true;

    try {
        pVariableL->GetValueRef() += pVariableR->GetValueRef();
    }
    catch (ExprException &ex) {
        state = false;
        pILRunState->SetError(ex);
    }

    return state;
}

void MatrixPlusILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixPlus";
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

bool MatrixMinusILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState)
{
    _ASSERT(NULL != pILRunState);

    bool state = true;

    try {
        pVariableL->GetValueRef() -= pVariableR->GetValueRef();
    }
    catch (ExprException &ex) {
        state = false;
        pILRunState->SetError(ex);
    }

    return state;
}

void MatrixMinusILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixMinus";
}

//---------------------------------------------------------------------
// Class member - MatrixMultiplyILCode
//---------------------------------------------------------------------
MatrixMultiplyILCode::MatrixMultiplyILCode()
{ }

ExprILCodeEnum MatrixMultiplyILCode::GetCodeEnum() const
{
    return EIL_MatrixMultiply;
}

bool MatrixMultiplyILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState)
{
    _ASSERT(NULL != pILRunState);

    bool state = true;

    try {
        pVariableL->GetValueRef() *= pVariableR->GetValueRef();
    }
    catch (ExprException &ex) {
        state = false;
        pILRunState->SetError(ex);
    }

    return state;
}

void MatrixMultiplyILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixMultiply";
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

bool MatrixDotMultiplyILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState)
{
    _ASSERT(NULL != pILRunState);

    bool state = true;

    try {
        pVariableL->GetValueRef().DotAssignmentMultiply(pVariableR->GetValueRef());
    }
    catch (ExprException &ex) {
        state = false;
        pILRunState->SetError(ex);
    }

    return state;
}

void MatrixDotMultiplyILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixDotMultiply";
}

//---------------------------------------------------------------------
// Class member - MatrixDivideILCode
//---------------------------------------------------------------------
MatrixDivideILCode::MatrixDivideILCode()
{ }

ExprILCodeEnum MatrixDivideILCode::GetCodeEnum() const
{
    return EIL_MatrixDivide;
}

bool MatrixDivideILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState)
{
    _ASSERT(NULL != pILRunState);

    bool state = true;

    try {
        (pVariableL->GetValueRef()) /= (pVariableR->GetValueRef());
    }
    catch (ExprException &ex) {
        state = false;
        pILRunState->SetError(ex);
    }

    return state;
}

void MatrixDivideILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixDivide";
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

bool MatrixDotDivideILCode::DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState)
{
    _ASSERT(NULL != pILRunState);

    bool state = true;

    try {
        pVariableL->GetValueRef().DotAssignmentDivid(pVariableR->GetValueRef());
    }
    catch (ExprException &ex) {
        state = false;
        pILRunState->SetError(ex);
    }

    return state;
}

void MatrixDotDivideILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixDotDivide";
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

    GetVariableStack(pILRunState)->TopVar(&pVariableR);
    GetVariableStack(pILRunState)->TopVar(&pVariableL, 1);
    if (RealVariable::TypeId == pVariableR->GetTypeId() && MatrixVariable::TypeId == pVariableL->GetTypeId()) {
        if (DoOperator((MatrixVariable*)pVariableL, (RealVariable*)pVariableR, pILRunState)) {
            GetVariableStack(pILRunState)->RemoveTopVar();

            return true;
        }
    }
    else
    {
        pILRunState->SetError("One or more operand is invalid!");
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

bool MatrixValPlusILCode::DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState)
{
    pVariableL->GetValueRef() += pVariableR->GetValue();

    return true;
}

void MatrixValPlusILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixValPlus";
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

bool MatrixValMinusILCode::DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState)
{
    pVariableL->GetValueRef() -= pVariableR->GetValue();

    return true;
}

void MatrixValMinusILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixValMinus";
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

bool MatrixValMultiplyILCode::DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState)
{
    pVariableL->GetValueRef() *= pVariableR->GetValue();

    return true;
}

void MatrixValMultiplyILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixValMultiply";
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

bool MatrixValDivideILCode::DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState)
{
    _ASSERT(NULL != pILRunState);

    bool state = true;

    try {
        pVariableL->GetValueRef() /= pVariableR->GetValue();
    }
    catch (ExprException &ex) {
        state = false;
        pILRunState->SetError(ex);
    }

    return state;
}

void MatrixValDivideILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixValDivide";
}