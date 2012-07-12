#ifndef BaseFunction_H
#define BaseFunction_H

#include <vector>
#include "FuncParamsInfo.h"
#include "VariableSet.h"
#include "Symbol.h"


class BaseFunction
{
public :
    // destructor
    virtual ~BaseFunction();
    // Methods
    virtual ResultTypeEnum GetReturnType(void) const = 0;
    virtual FuncParamsInfo GetFuncInfo(void) const = 0;
    virtual Variable* Call(std::vector<Variable*> *pVarVec) = 0;
protected :
    // Constructor
    BaseFunction(void);
};

struct FunctionInfo
{
    BaseFunction* (*_pfnCreateFuncInst)();
    FunctionInfo *_pNextFuncInfo;

    BaseFunction* CreateFuncInst() const;

    static FunctionInfo *s_pFirstFuncInfo;
};

struct FUNCTIONINFO_INIT
{
    FUNCTIONINFO_INIT(FunctionInfo *pFunctionInfo);
};

#define DECLARE_FUNCINFO(class_name) \
    public : \
        static FunctionInfo s_FuncInfo##class_name; \
        static BaseFunction* CreateFuncInst();

#define IMPLEMENT_FUNCINFO(class_name) \
    BaseFunction* class_name::CreateFuncInst() \
    { return new class_name(); } \
    FunctionInfo class_name::s_FuncInfo##class_name = { \
        class_name::CreateFuncInst, NULL \
        }; \
    FUNCTIONINFO_INIT _init_##class_name(&class_name::s_FuncInfo##class_name)

#endif