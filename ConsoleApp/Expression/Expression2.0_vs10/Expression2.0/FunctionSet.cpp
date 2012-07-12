#include "stdafx.h"
#include "FunctionSet.h"
#include <crtdbg.h>
using namespace std;

void FunctionSet::AddFunctionItem(const FuncParamsInfo &funcInfo, ExprILCode_sp spILCode)
{
    _ASSERT((bool)spILCode);

    FuncMapIter_t it = _FuncMap.find(funcInfo);

    if (_FuncMap.end() != it) {
        // If the function info is existing.
        // Delete original one.
        // Add the point of new one into _FuncMap
        it->second = spILCode;
    }
    else {
        // If the function info is not existing.
        // Add the point of new one into _FuncMap.
        // Add the function name into _FuncNameMap.
        _FuncMap[funcInfo] = spILCode;
        ++_FuncNameMap[funcInfo.GetFuncName()];
    }
}

ExprILCode_sp FunctionSet::FindFunctionItem(const FuncParamsInfo &funcInfo)
{
    FuncMapIter_t it = _FuncMap.find(funcInfo);

    return (_FuncMap.end() != it ? it->second : ExprILCode_sp(nullptr));
}

int FunctionSet::FindFunctionName(const std::string &funcName) const
{
    FuncNameMapCIter_t it = _FuncNameMap.find(funcName);

    return (_FuncNameMap.end() != it ? it->second : 0);
}