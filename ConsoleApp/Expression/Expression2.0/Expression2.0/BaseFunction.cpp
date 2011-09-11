#include "stdafx.h"
#include "BaseFunction.h"
#include <crtdbg.h>
using namespace std;


BaseFunction::BaseFunction(FuncParamsInfo funcInfo) :
_FuncInfo(funcInfo)
{ }

BaseFunction::~BaseFunction()
{ }

FunctionInfo* FunctionInfo::s_pFirstFuncInfo = NULL;

BaseFunction* FunctionInfo::CreateFuncInst() const
{
    _ASSERT(NULL != _pfnCreateFuncInst);

    return _pfnCreateFuncInst();
}

FUNCTIONINFO_INIT::FUNCTIONINFO_INIT(FunctionInfo *pFunctionInfo)
{
    pFunctionInfo->_pNextFuncInfo = FunctionInfo::s_pFirstFuncInfo;
    FunctionInfo::s_pFirstFuncInfo = pFunctionInfo;
}