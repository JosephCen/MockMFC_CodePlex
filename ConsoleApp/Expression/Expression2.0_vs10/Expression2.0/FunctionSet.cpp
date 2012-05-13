#include "stdafx.h"
#include "FunctionSet.h"
#include <crtdbg.h>
using namespace std;

FunctionSet::~FunctionSet()
{
    for (FuncMapIter_t it = _FuncMap.begin(); _FuncMap.end() != it; ++it) {
        if (this == it->second->GetOwner())
            delete it->second;
        it->second = NULL;
    }
}

void FunctionSet::AddFunctionItem(const FuncParamsInfo &funcInfo, ExprILCode *pILCode)
{
    _ASSERT(NULL != pILCode);

    FuncMapIter_t it = _FuncMap.find(funcInfo);

    if (_FuncMap.end() != it) {
        // If the function info is existing.
        // Delete original one.
        // Add the point of new one into _FuncMap
        if (this == it->second->GetOwner())
            delete it->second;
        it->second = pILCode;
    }
    else {
        // If the function info is not existing.
        // Add the point of new one into _FuncMap.
        // Add the function name into _FuncNameMap.
        pILCode->SetOwner(this);
        _FuncMap[funcInfo] = pILCode;
        ++_FuncNameMap[funcInfo.GetFuncName()];
    }
}

ExprILCode* FunctionSet::FindFunctionItem(const FuncParamsInfo &funcInfo)
{
    FuncMapIter_t it = _FuncMap.find(funcInfo);

    return (_FuncMap.end() != it ? it->second : NULL);
}

int FunctionSet::FindFunctionName(const std::string &funcName) const
{
    FuncNameMapCIter_t it = _FuncNameMap.find(funcName);

    return (_FuncNameMap.end() != it ? it->second : 0);
}