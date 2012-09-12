#include "stdafx.h"
#include "FunctionNT.h"
#include "ExprNT.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - FunctionNT
//---------------------------------------------------------------------
bool FunctionNT::IsInFirstSet(WordTypeEnum wordType)
{
    // function => deffunc (expr, ...)
    return WT_DefFunction == wordType;
}

FunctionNT::FunctionNT(void):
_ExprVec(), _pExprILCode(nullptr), BaseNonTerminal()
{ }

FunctionNT::~FunctionNT()
{
    // This _pExprILCode point to a pointer stored in container FunctionSet.
    _pExprILCode = nullptr;
	for (auto iter = _ExprVec.begin(); _ExprVec.end() != iter; ++iter) {
		delete *iter;
		*iter = nullptr;
	}
}

bool FunctionNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    _ASSERT(IsInFirstSet(wordCursorRef.CurrentWord().WordType()));

    bool isSuccess = true;
    int operatorWordIdx = wordCursorRef.CurrentIdx();
    string funcName = wordCursorRef.CurrentWord().StringValue();

    isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
    if (isSuccess) {
        if (WT_Paranthese_L != wordCursorRef.CurrentWord().WordType()) {
            exprContextRef.SetError("Function call missing left bracket of argument list.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }
    isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
    if (ExprNT::IsInFirstSet(wordCursorRef.CurrentWord().WordType())) {
        while (isSuccess) {
            _ExprVec.push_back(new ExprNT());
            isSuccess = isSuccess && _ExprVec.back()->Parse(exprContextRef, wordCursorRef);
            if (isSuccess && WT_Comma == wordCursorRef.CurrentWord().WordType())
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            else
                break;
        }
    }
    if (isSuccess) {
        if (WT_Paranthese_R != wordCursorRef.CurrentWord().WordType()) {
            exprContextRef.SetError("Function call missing right bracket of argument list.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }
    isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
    isSuccess = isSuccess && OperatorValidate(exprContextRef, funcName, operatorWordIdx);

    return isSuccess;
}

bool FunctionNT::OperatorValidate(ExprContext &exprContextRef, std::string &funcName, int operWordIdx)
{
    vector<int> paramVec;

    for (ExprVecIter_t iter = _ExprVec.begin(); _ExprVec.end() != iter; ++iter)
        paramVec.push_back((*iter)->ResultType());

    FuncParamsInfo funcInfo(funcName.c_str(), paramVec);

    _pExprILCode = FindExprILCode(funcInfo);
    if (nullptr != _pExprILCode) {
        return true;
    }
    else {
        exprContextRef.SetError("Cannot find overloaded function for given arguments", operWordIdx);

        return false;
    }
}

ResultTypeEnum FunctionNT::GetResultType(void)
{
    _ASSERT(nullptr != _pExprILCode);

    return _pExprILCode->GetReturnType();
}

ExprILCodeSegment& FunctionNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    for (ExprVecIter_t iter = _ExprVec.begin(); _ExprVec.end() != iter; ++iter)
        (*iter)->AppendILSegment(ilSegment);
    ilSegment.Append(_pExprILCode->Duplicate());

    return ilSegment;
}
