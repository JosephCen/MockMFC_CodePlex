#ifndef ExprILHelper_H
#define ExprILHelper_H

#include "WordParser.h"
#include "ExprILCode.h"
#include "FunctionSet.h"
#include <string>

// This is a static class, which is used to provide some static help methods.
class ExprILHelper
{
private :
    static bool s_IsInitialized;
    static FunctionSet s_OperatorSet;

    // Constructor
    ExprILHelper(); // Avoid another code create an instance of this class by mistake.
public :
    // Methods
    static ExprILCode* FindOperatorILCode(const FuncParamsInfo &funcInfo);
    static ExprILCode* FindOperatorILCode(WordTypeEnum operWordType, ResultTypeEnum lParamType, ResultTypeEnum rParamType);
    static std::string GenOperNotFoundErr(WordTypeEnum operWordType, ResultTypeEnum lParamType, ResultTypeEnum rParamType);
private :
    // Private methods
    static void Initialize();
    static const char* GetOperatorStr(WordTypeEnum operWordType);
    static const char* GetResultTypeName(ResultTypeEnum paramType);
};

inline ExprILCode* ExprILHelper::FindOperatorILCode(const FuncParamsInfo &funcInfo)
{
    if (!s_IsInitialized)
        Initialize();

    return (s_OperatorSet.FindFunctionItem(funcInfo));
}

#endif