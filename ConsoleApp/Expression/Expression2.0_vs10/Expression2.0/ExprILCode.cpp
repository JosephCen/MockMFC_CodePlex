#include "stdafx.h"
#include "ExprILCode.h"
#include "ExprException.h"
#include "DoubleComparison.h"
#include <vector>
#include <string>
#include <sstream>
#include <crtdbg.h>
using namespace std;

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
ExprILCode::ExprILCode(void)
{ }

ExprILCode::~ExprILCode()
{ }

ResultTypeEnum ExprILCode::GetReturnType(void) const
{
    return RT_None;
}

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

ResultTypeEnum PushIntegerILCode::GetReturnType(void) const
{
    return RT_Integer;
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

ResultTypeEnum PushRealValILCode::GetReturnType(void) const
{
    return RT_RealVal;
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
    Variable_sp spVariable = pILRunState->GetVariableSet()->SearchVar(_DefValName);

    if ((bool)spVariable) {
        pILRunState->GetVariableStack()->PushVar(spVariable->Duplicate());

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
// ReverseBinaryOperILCode - class
//---------------------------------------------------------------------
ReverseBinaryOperILCode::ReverseBinaryOperILCode(ExprILCode *pBinaryILCode) :
_pBinaryILCode(pBinaryILCode)
{
    _ASSERT(nullptr != pBinaryILCode);
}

ReverseBinaryOperILCode::~ReverseBinaryOperILCode()
{
    delete _pBinaryILCode;
    _pBinaryILCode = NULL;
}

ExprILCodeEnum ReverseBinaryOperILCode::GetCodeEnum() const
{
    return (_pBinaryILCode->GetCodeEnum());
}

ResultTypeEnum ReverseBinaryOperILCode::GetReturnType(void) const
{
    return (_pBinaryILCode->GetReturnType());
}

bool ReverseBinaryOperILCode::RunCode(ExprILRunState *pILRunState)
{
    _ASSERT(GetVariableStack(pILRunState)->Count() >= 2);

    Variable *pVariable1 = NULL;
    Variable *pVariable2 = NULL;

    GetVariableStack(pILRunState)->PopVar(&pVariable1);
    GetVariableStack(pILRunState)->PopVar(&pVariable2);
    GetVariableStack(pILRunState)->PushVar(pVariable1);
    GetVariableStack(pILRunState)->PushVar(pVariable2);

    return (_pBinaryILCode->RunCode(pILRunState));
}

void ReverseBinaryOperILCode::ToString(ostream *pOStream) const
{
    *pOStream << "Reverse ";
    _pBinaryILCode->ToString(pOStream);
}

//---------------------------------------------------------------------
// Class member - RealValBinaryOperILCode
//---------------------------------------------------------------------
RealValBinaryOperILCode::RealValBinaryOperILCode(void)
{ }

ResultTypeEnum RealValBinaryOperILCode::GetReturnType(void) const
{
    return RT_RealVal;
}

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
RealValPlusILCode::RealValPlusILCode(void)
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
RealValMinusILCode::RealValMinusILCode(void)
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
RealValMultiplyILCode::RealValMultiplyILCode(void)
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
RealValDivideILCode::RealValDivideILCode(void)
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

ResultTypeEnum CtorMatrixILCode::GetReturnType(void) const
{
    return RT_Matrix;
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

ResultTypeEnum MatrixBinaryOperILCode::GetReturnType() const
{
    return RT_Matrix;
}

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

ResultTypeEnum MatrixValBinaryOperILCode::GetReturnType(void) const
{
    return RT_Matrix;
}

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

//---------------------------------------------------------------------
// Class member - CallFunctionILCode
//---------------------------------------------------------------------
CallFunctionILCode::CallFunctionILCode(BaseFunction *pFunc) :
_pFunc(pFunc)
{
	_ASSERT(nullptr != pFunc);
}

CallFunctionILCode::~CallFunctionILCode()
{
	delete _pFunc;
	_pFunc = nullptr;
}

ExprILCodeEnum CallFunctionILCode::GetCodeEnum() const
{
    return EIL_CallFunction;
}

ResultTypeEnum CallFunctionILCode::GetReturnType(void) const
{
    return _pFunc->GetReturnType();
}

bool CallFunctionILCode::RunCode(ExprILRunState *pILRunState)
{
    bool state = true;
    int paramCount = _pFunc->GetFuncInfo().GetParamCount();
    vector<Variable*> paramVec(paramCount, static_cast<Variable*>(NULL));
    Variable* pRetVariable = NULL;

    for (int i = 0; i < paramCount; ++i)
        GetVariableStack(pILRunState)->TopVar(&(paramVec[paramCount - 1 - i]), i);

    try {
        pRetVariable = _pFunc->Call(&paramVec);
    }
    catch (ExprException &ex) {
        state = false;
        pILRunState->SetError(ex);
    }

    if (state) {
        GetVariableStack(pILRunState)->RemoveTopVar(paramCount);
        if (NULL != pRetVariable)
            GetVariableStack(pILRunState)->PushVar(pRetVariable);
    }

    return state;
}

void CallFunctionILCode::ToString(ostream *pOStream) const
{
    *pOStream << "CallFunction";
}