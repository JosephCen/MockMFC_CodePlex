#include "stdafx.h"
#include "FuncParamsInfo.h"
#include <crtdbg.h>
using namespace std;

FuncParamsInfo::FuncParamsInfo(const char *funcName, int paramType0) :
_FuncName(funcName)
{
    _ASSERT(_FuncName.length() > 0);

    _ParamTypes.push_back(paramType0);
}

FuncParamsInfo::FuncParamsInfo(const char *funcName, int paramType0, int paramType1) :
_FuncName(funcName)
{
    _ASSERT(_FuncName.length() > 0);

    _ParamTypes.push_back(paramType0);
    _ParamTypes.push_back(paramType1);
}

FuncParamsInfo::FuncParamsInfo(const char *funcName, const std::vector<int> &paramTypeVec) :
_FuncName(funcName), _ParamTypes(paramTypeVec)
{
    _ASSERT(_FuncName.length() > 0);
}

bool operator < (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal)
{
    int funcNameCmpRet = lVal._FuncName.compare(rVal._FuncName);

    if (funcNameCmpRet != 0)
        return (funcNameCmpRet < 0 ? true : false);

    if (lVal._ParamTypes.size() != rVal._ParamTypes.size())
        return (lVal._ParamTypes.size() < rVal._ParamTypes.size() ? true : false);

    for (vector<int>::size_type i = 0; i < lVal._ParamTypes.size(); ++i) {
        if (lVal._ParamTypes[i] != rVal._ParamTypes[i])
            return (lVal._ParamTypes[i] < rVal._ParamTypes[i] ? true : false);
    }

    return false;
}

bool operator == (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal)
{
    if (lVal._FuncName != rVal._FuncName)
        return false;

    if (lVal._ParamTypes.size() != rVal._ParamTypes.size())
        return false;

    for (vector<int>::size_type i = 0; i < lVal._ParamTypes.size(); ++i) {
        if (lVal._ParamTypes[i] != rVal._ParamTypes[i])
            return false;
    }

    return true;
}