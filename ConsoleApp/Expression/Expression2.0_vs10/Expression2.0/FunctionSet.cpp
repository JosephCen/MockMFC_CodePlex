#include "stdafx.h"
#include "FunctionSet.h"
#include <algorithm>
#include <crtdbg.h>
using namespace std;

FunctionSet::~FunctionSet()
{
    Clear();
}

void FunctionSet::AddFunctionItem(const FuncParamsInfo &funcInfo, ExprILCode *pILCode)
{
    _ASSERT_EXPR(nullptr != pILCode, L"pILCode should not point to a NULL pointer.");

    ExprILCode* &pILRef = _FuncMap[funcInfo];

    if (nullptr != pILRef) {
        // If the function info is existing.
        // Delete original one.
        // Add the point of new one into _FuncMap
        delete pILRef;
        pILRef = pILCode;
    }
    else {
        // If the function info is not existing.
        // Add the point of new one into _FuncMap.
        // Add the function name into _FuncNameMap.
        pILRef = pILCode;
        ++_FuncNameMap[funcInfo.GetFuncName()];
    }
}

ExprILCode* FunctionSet::FindFunctionItem(const FuncParamsInfo &funcInfo)
{
    FuncMapIter_t it = _FuncMap.find(funcInfo);

    return (_FuncMap.end() != it ? it->second : nullptr);
}

int FunctionSet::FindFunctionName(const std::string &funcName) const
{
    FuncNameMapCIter_t it = _FuncNameMap.find(funcName);

    return (_FuncNameMap.end() != it ? it->second : 0);
}

void FunctionSet::Clear()
{
    for_each(_FuncMap.begin(), _FuncMap.end(), [] (FuncMapVal_t &vRef) {
        delete vRef.second;
        vRef.second = nullptr;
    });
	_FuncMap.clear();
	_FuncNameMap.clear();
}