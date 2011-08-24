#ifndef FunctionSet_H
#define FunctionSet_H

#include "ExprILCode.h"
#include "FuncParamsInfo.h"
#include <map>

class FunctionSet
{
private :
    typedef std::map<FuncParamsInfo, ExprILCode*>::iterator FuncMapIter_t;
    typedef std::map<FuncParamsInfo, ExprILCode*>::const_iterator FuncMapCIter_t;
private :
    std::map<FuncParamsInfo, ExprILCode*> _FuncMap;
public :
    // Construct
    virtual ~FunctionSet();
    // Methods
    void AddFunctionItem(const FuncParamsInfo &funcInfo, ExprILCode *pILCode);
    ExprILCode* FindFunctionItem(const FuncParamsInfo &funcInfo);
};

#endif