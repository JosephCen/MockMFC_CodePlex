#ifndef BaseFunction_H
#define BaseFunction_H

#include <vector>
#include "SharedPtr.h"
#include "FuncParamsInfo.h"
#include "VariableSet.h"
#include "Symbol.h"


class BaseFunction
{
private :
    FuncParamsInfo _FuncInfo;
public :
    // destructor
    virtual ~BaseFunction();
    // Methods
    virtual ResultTypeEnum GetReturnType(void) const = 0;
    const FuncParamsInfo& GetFuncInfo(void) const;
    virtual Variable* Call(std::vector<Variable*> *pVarVec) = 0;
protected :
    // Constructor
    BaseFunction(FuncParamsInfo funcInfo);
};

typedef SharedPtr<BaseFunction> BaseFunction_sp;

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


inline const FuncParamsInfo& BaseFunction::GetFuncInfo(void) const
{
    return _FuncInfo;
}

#endif