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
    // The purpose for this method is used to avoid these empty 'GetReturnType()' methods defined 
    // in derived class of ExprILCode.
    _ASSERT_EXPR(0, L"Method 'ExprILCode::GetReturnType()' should not be called directly.");

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

IMPLEMENT_EILDUP(PushIntegerILCode)

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

IMPLEMENT_EILDUP(PushRealValILCode)

void PushRealValILCode::ToString(ostream *pOStream) const
{
    *pOStream << "PushRealVal " << _RealValue;
}

//---------------------------------------------------------------------
// PushDefValILCode - class
//---------------------------------------------------------------------
PushDefValILCode::PushDefValILCode(const string &defValName, ResultTypeEnum resultType) :
_DefValName(defValName), _ResultType(resultType)
{ }

ExprILCodeEnum PushDefValILCode::GetCodeEnum() const
{
    return EIL_PushDefVal;
}

ResultTypeEnum PushDefValILCode::GetReturnType(void) const
{
    return _ResultType;
}

bool PushDefValILCode::RunCode(ExprILRunState *pILRunState)
{
    Variable_sp spVariable = pILRunState->GetVariableSet()->SearchVar(_DefValName);

    if ((bool)spVariable) {
		_ASSERT(spVariable->GetTypeId() == (int)_ResultType); // The type of varialbe should not be changed.

        pILRunState->GetVariableStack()->PushVar(spVariable->Duplicate());

        return true;
    }
    else {
        pILRunState->SetError("Defined variable cannot be found.");
    }

    return false;
}

IMPLEMENT_EILDUP(PushDefValILCode)

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

ReverseBinaryOperILCode::ReverseBinaryOperILCode(const ReverseBinaryOperILCode &otherRef)
{
    _pBinaryILCode = otherRef._pBinaryILCode->Duplicate();
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

IMPLEMENT_EILDUP(ReverseBinaryOperILCode)

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

IMPLEMENT_EILDUP(RealValPlusILCode)

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

IMPLEMENT_EILDUP(RealValMinusILCode)

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

IMPLEMENT_EILDUP(RealValMultiplyILCode)

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

IMPLEMENT_EILDUP(RealValDivideILCode)

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

IMPLEMENT_EILDUP(CtorMatrixILCode)

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

IMPLEMENT_EILDUP(MatrixPlusILCode)

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

IMPLEMENT_EILDUP(MatrixMinusILCode)

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

IMPLEMENT_EILDUP(MatrixMultiplyILCode)

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

IMPLEMENT_EILDUP(MatrixDotMultiplyILCode)

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

IMPLEMENT_EILDUP(MatrixDivideILCode)

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

IMPLEMENT_EILDUP(MatrixDotDivideILCode)

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

IMPLEMENT_EILDUP(MatrixValPlusILCode)

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

IMPLEMENT_EILDUP(MatrixValMinusILCode)

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

IMPLEMENT_EILDUP(MatrixValMultiplyILCode)

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

IMPLEMENT_EILDUP(MatrixValDivideILCode)

void MatrixValDivideILCode::ToString(ostream *pOStream) const
{
    *pOStream << "MatrixValDivide";
}

//---------------------------------------------------------------------
// Class member - CallFunctionILCode
//---------------------------------------------------------------------
CallFunctionILCode::CallFunctionILCode(FunctionInfo *pFuncInfo, BaseFunction *pFunc) :
_pFuncInfo(pFuncInfo), _pFunc(pFunc)
{
	_ASSERT(nullptr != pFuncInfo);
    _ASSERT(nullptr != pFunc);
}

CallFunctionILCode::CallFunctionILCode(const CallFunctionILCode &otherRef) :
_pFuncInfo(otherRef._pFuncInfo), _pFunc(nullptr)
{
    _pFunc = _pFuncInfo->CreateFuncInst();
}

CallFunctionILCode::~CallFunctionILCode()
{
	delete _pFunc;
	_pFunc = nullptr;
    _pFuncInfo = nullptr;
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

IMPLEMENT_EILDUP(CallFunctionILCode)

void CallFunctionILCode::ToString(ostream *pOStream) const
{
    *pOStream << "CallFunction";
}

//---------------------------------------------------------------------
// Class member - NewVariableILCode
//---------------------------------------------------------------------
NewVariableILCode::NewVariableILCode(const string &variableName) :
_VariableName(variableName)
{
    _ASSERT_EXPR(_VariableName.length() > 0, L"Name of variable should contain at lease 1 character.");
}

ExprILCodeEnum NewVariableILCode::GetCodeEnum() const
{
    return EIL_NewVariable;
}

bool NewVariableILCode::RunCode(ExprILRunState *pILRunState)
{
    _ASSERT_EXPR(GetVariableStack(pILRunState)->Count() > 0, L"There should be at lease 1 variable in the variable stack.");

    Variable *pVariable = nullptr;

    GetVariableStack(pILRunState)->TopVar(&pVariable);
    pILRunState->GetVariableSet()->InsertVar(_VariableName, Variable_sp(pVariable->Duplicate()));

    return true;
}

IMPLEMENT_EILDUP(NewVariableILCode)

void NewVariableILCode::ToString(ostream *pOStream) const
{
    *pOStream << "NewVariable " << _VariableName;
}