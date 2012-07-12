#ifndef FunctionSet_H
#define FunctionSet_H

#include "ExprILCode.h"
#include "FuncParamsInfo.h"
#include <map>

class FunctionSet
{
private :
    typedef std::map<FuncParamsInfo, ExprILCode_sp> FuncMap_t;
    typedef std::map<FuncParamsInfo, ExprILCode_sp>::iterator FuncMapIter_t;
    typedef std::map<FuncParamsInfo, ExprILCode_sp>::const_iterator FuncMapCIter_t;
    typedef std::map<std::string, int> FuncNameMap_t;
    typedef std::map<std::string, int>::iterator FuncNameMapIter_t;
    typedef std::map<std::string, int>::const_iterator FuncNameMapCIter_t;
private :
    FuncMap_t _FuncMap;
    FuncNameMap_t _FuncNameMap;
public :
    // Methods
    void AddFunctionItem(const FuncParamsInfo &funcInfo, ExprILCode_sp spILCode);
    ExprILCode_sp FindFunctionItem(const FuncParamsInfo &funcInfo);
    int FindFunctionName(const std::string &funcName) const;
};

#endif