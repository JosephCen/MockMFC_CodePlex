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
    static FunctionSet s_OperatorSet; // Set of all these internal operator and function.

    // Constructor
    ExprILHelper(); // Avoid another code create an instance of this class by mistake.
public :
    // Methods
    static ExprILCode* FindOperatorILCode(const FuncParamsInfo &funcInfo);
    static int FindOperatorILCount(const std::string &funcName);
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

inline int ExprILHelper::FindOperatorILCount(const std::string &funcName)
{
    if (!s_IsInitialized)
        Initialize();

    return (s_OperatorSet.FindFunctionName(funcName));
}

#endif