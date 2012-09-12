#include "stdafx.h"
#include "SubExprNT.h"
#include "ExprILHelper.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - SubExprNT
//---------------------------------------------------------------------
SubExprNT::SubExprNT(void):
BaseNonTerminal(), _IsFirstOne(true), _pLeftOne(NULL), _pExprILCode(nullptr), _TermNT()
{ }

SubExprNT::SubExprNT(SubExprNT *pLeftOne):
BaseNonTerminal(), _IsFirstOne(false), _pLeftOne(pLeftOne), _pExprILCode(nullptr), _TermNT()
{
    _ASSERT(NULL != pLeftOne);
}

SubExprNT::~SubExprNT()
{
    // This _pExprILCode point to a pointer stored in container FunctionSet.
    _pExprILCode = nullptr;
    delete _pLeftOne;
    _pLeftOne = NULL;
}

bool SubExprNT::IsInFirstSet(WordTypeEnum wordType, bool isFirstOne)
{
    // expr => term moreterms
    // moreterms => + term moreterms
    //            | - term moreterms
    //            | nul

    if (isFirstOne)
        return TermNT::IsInFirstSet(wordType);
    else
        return WT_Plus == wordType || WT_Minus == wordType;
}

bool SubExprNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // expr => term moreterms
    // moreterms => + term moreterms
    //            | - term moreterms
    //            | nul

    if (_IsFirstOne) {
        // => term

        isSuccess = isSuccess && _TermNT.Parse(exprContextRef, wordCursorRef);
    }
    else {
        // => + term moreterms
        //  | - term moreterms
        //  | nul

        if (IsInFirstSet(wordCursorRef.CurrentWord().WordType(), false)) {
            int operatorWordIdx = wordCursorRef.CurrentIdx();
            WordTypeEnum operatorWordType = wordCursorRef.CurrentWord().WordType();

            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            isSuccess = isSuccess && _TermNT.Parse(exprContextRef, wordCursorRef);
            isSuccess = isSuccess && OperatorValidate(exprContextRef, operatorWordType, operatorWordIdx);
        }
        else {
            exprContextRef.SetError("Expect a plus or a minus operator.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }

    return isSuccess;
}

bool SubExprNT::OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx)
{
    _pExprILCode = FindExprILCode(operWordType, _pLeftOne->ResultType(), _TermNT.ResultType());

    if (nullptr != _pExprILCode) {
        return true;
    }
    else {
        string errStr = ExprILHelper::GenOperNotFoundErr(operWordType, _pLeftOne->ResultType(), _TermNT.ResultType());

        exprContextRef.SetError(errStr.c_str(), operWordIdx);

        return false;
    }
}

ResultTypeEnum SubExprNT::GetResultType(void)
{
    if (_IsFirstOne) {
        return _TermNT.ResultType();
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        return (_pExprILCode->GetReturnType());
    }
}

SubExprNT* SubExprNT::GetLeftSubExpr()
{
    return _pLeftOne;
}

TermNT* SubExprNT::GetTerm()
{
    return &_TermNT;
}

ExprILCodeSegment& SubExprNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    if (_IsFirstOne) {
        _TermNT.AppendILSegment(ilSegment);
    }
    else {
        _ASSERT(NULL != _pLeftOne);
        _ASSERT(nullptr != _pExprILCode);

        _pLeftOne->AppendILSegment(ilSegment);
        _TermNT.AppendILSegment(ilSegment);
        ilSegment.Append(_pExprILCode->Duplicate());
    }

    return ilSegment;
}
