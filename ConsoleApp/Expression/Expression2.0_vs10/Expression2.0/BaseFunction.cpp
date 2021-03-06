#include "stdafx.h"
#include "BaseFunction.h"
#include <crtdbg.h>
using namespace std;
using namespace std::tr1;


BaseFunction::BaseFunction(void)
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