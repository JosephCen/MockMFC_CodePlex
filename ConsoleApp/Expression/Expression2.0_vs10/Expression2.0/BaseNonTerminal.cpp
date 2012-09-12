#include "stdafx.h"
#include "BaseNonTerminal.h"
#include "StartNT.h"
#include "ExprILHelper.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - BaseNonTerminal
//---------------------------------------------------------------------
BaseNonTerminal* BaseNonTerminal::NewStartNT()
{
    return new StartNT();
}

BaseNonTerminal::BaseNonTerminal(void) :
_ResultType(RT_None)
{ }

BaseNonTerminal::BaseNonTerminal(BaseNonTerminal &&rValRef) :
_ResultType(rValRef._ResultType)
{
    rValRef._ResultType = RT_None;
}

ResultTypeEnum BaseNonTerminal::GetResultType(void)
{
    _ASSERT(0); return RT_None;
}

ResultTypeEnum BaseNonTerminal::ResultType(void)
{
    if (RT_None == _ResultType)
        _ResultType = GetResultType();

    return _ResultType;
}

ExprILCode* BaseNonTerminal::FindExprILCode(const FuncParamsInfo &funcInfo)
{
    // TODO: Add logic of override operator

    return ExprILHelper::FindOperatorILCode(funcInfo);
}

ExprILCode* BaseNonTerminal::FindExprILCode(WordTypeEnum operWordType, ResultTypeEnum lParamType, ResultTypeEnum rParamType)
{
    return ExprILHelper::FindOperatorILCode(operWordType, lParamType, rParamType);
}
