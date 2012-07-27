#include "stdafx.h"
#include "ExprILHelper.h"
#include <sstream>
#include <crtdbg.h>
using namespace std;

bool ExprILHelper::s_IsInitialized = false;
FunctionSet ExprILHelper::s_OperatorSet;


void ExprILHelper::Initialize()
{
    if (!s_IsInitialized) {
        // RealValue & RealValue
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("+", RealVariable::TypeId, RealVariable::TypeId),
                                      new RealValPlusILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("-", RealVariable::TypeId, RealVariable::TypeId),
                                      new RealValMinusILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("*", RealVariable::TypeId, RealVariable::TypeId),
                                      new RealValMultiplyILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("/", RealVariable::TypeId, RealVariable::TypeId),
                                      new RealValDivideILCode());
        // Matrix & Matrix
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("+", MatrixVariable::TypeId, MatrixVariable::TypeId),
                                      new MatrixPlusILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("-", MatrixVariable::TypeId, MatrixVariable::TypeId),
                                      new MatrixMinusILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("*", MatrixVariable::TypeId, MatrixVariable::TypeId),
                                      new MatrixMultiplyILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo(".*", MatrixVariable::TypeId, MatrixVariable::TypeId),
                                      new MatrixDotMultiplyILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("/", MatrixVariable::TypeId, MatrixVariable::TypeId),
                                      new MatrixDivideILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("./", MatrixVariable::TypeId, MatrixVariable::TypeId),
                                      new MatrixDotDivideILCode());
        // Matrix & RealValue
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("+", MatrixVariable::TypeId, RealVariable::TypeId),
                                      new MatrixValPlusILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("+", RealVariable::TypeId, MatrixVariable::TypeId),
                                      new ReverseBinaryOperILCode(new MatrixValPlusILCode()));
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("-", MatrixVariable::TypeId, RealVariable::TypeId),
                                      new MatrixValMinusILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("*", MatrixVariable::TypeId, RealVariable::TypeId),
                                      new MatrixValMultiplyILCode());
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("*", RealVariable::TypeId, MatrixVariable::TypeId),
                                      new ReverseBinaryOperILCode(new MatrixValMultiplyILCode()));
        s_OperatorSet.AddFunctionItem(FuncParamsInfo("/", MatrixVariable::TypeId, RealVariable::TypeId),
                                      new MatrixValDivideILCode());

        // Function
        FunctionInfo *pFunctionInfo = FunctionInfo::s_pFirstFuncInfo;

        while (NULL != pFunctionInfo)
        {
            BaseFunction* pFunction = pFunctionInfo->CreateFuncInst();

            s_OperatorSet.AddFunctionItem(pFunction->GetFuncInfo(), new CallFunctionILCode(pFunctionInfo, pFunction));
            pFunctionInfo = pFunctionInfo->_pNextFuncInfo;
        }

        s_IsInitialized = true;
    }
}

const char* ExprILHelper::GetOperatorStr(WordTypeEnum operWordType)
{
    const char *operStr = NULL;

    switch (operWordType)
    {
    case WT_Plus :
        operStr = "+";
        break;
    case WT_Minus :
        operStr = "-";
        break;
    case WT_Multiply :
        operStr = "*";
        break;
    case WT_Divide :
        operStr = "/";
        break;
    case WT_DotMultiply :
        operStr = ".*";
        break;
    case WT_DotDivide :
        operStr = "./";
        break;
    default :
        _ASSERT(0); // Undefined operator
        break;
    }

    return operStr;
}

const char* ExprILHelper::GetResultTypeName(ResultTypeEnum paramType)
{
    const char *typeNameStr = NULL;

    switch (paramType)
    {
    case RT_String :
        typeNameStr = "string";
        break;
    case RT_Matrix :
        typeNameStr = "matrix";
        break;
    case RT_Integer :
        typeNameStr = "integer";
        break;
    case RT_RealVal :
        typeNameStr = "real value";
        break;
    default :
        _ASSERT(0); // Unknown type name
        break;
    }

    return typeNameStr;
}

ExprILCode* ExprILHelper::FindOperatorILCode(WordTypeEnum operWordType, ResultTypeEnum lParamType, ResultTypeEnum rParamType)
{
    FuncParamsInfo funcInfo(GetOperatorStr(operWordType), lParamType, rParamType);

    return FindOperatorILCode(funcInfo);
}

string ExprILHelper::GenOperNotFoundErr(WordTypeEnum operWordType, ResultTypeEnum lParamType, ResultTypeEnum rParamType)
{
    ostringstream errStrStream;

    if (lParamType == rParamType) {
        errStrStream << "Operator '" << GetOperatorStr(operWordType) << "' cannot have two '"
                     << GetResultTypeName(lParamType) << "' as operands.";
    }
    else {
        errStrStream << "Operator '" << GetOperatorStr(operWordType) << "' cannot have a '"
                     << GetResultTypeName(lParamType) << "' as left operand and a '"
                     << GetResultTypeName(rParamType) << "' as right operand.";
    }

    return (errStrStream.str());
}