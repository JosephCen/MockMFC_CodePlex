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
        if (this == it->second->GetOwner())
            delete it->second;
        it->second = pILCode;
    }
    else {
        pILCode->SetOwner(this);
        _FuncMap[funcInfo] = pILCode;
    }
}

ExprILCode* FunctionSet::FindFunctionItem(const FuncParamsInfo &funcInfo)
{
    FuncMapIter_t it = _FuncMap.find(funcInfo);

    return (_FuncMap.end() != it ? it->second : NULL);
}
