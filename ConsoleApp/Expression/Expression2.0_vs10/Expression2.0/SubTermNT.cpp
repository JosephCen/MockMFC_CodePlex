#include "stdafx.h"
#include "SubTermNT.h"
#include "ExprILHelper.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - SubTermNT
//---------------------------------------------------------------------
SubTermNT::SubTermNT(void):
BaseNonTerminal(), _IsFirstOne(true), _pLeftOne(NULL), _pExprILCode(nullptr), _FactorNT()
{ }

SubTermNT::SubTermNT(SubTermNT *pLeftOne):
BaseNonTerminal(), _IsFirstOne(false), _pLeftOne(pLeftOne), _pExprILCode(nullptr), _FactorNT()
{
    _ASSERT(NULL != pLeftOne);
}

SubTermNT::~SubTermNT()
{
    delete _pLeftOne;
    _pLeftOne = NULL;
}

bool SubTermNT::IsInFirstSet(WordTypeEnum wordType, bool isFirstOne)
{
    // term => factor morefactors
    // morefactors => * factor morefactors
    //              | / factor morefactors
    //              | .* factor morefactors
    //              | ./ factor morefactors
    //              | nul

    if (isFirstOne)
        return FactorNT::IsInFirstSet(wordType);
    else
        return WT_Multiply == wordType || WT_Divide == wordType || WT_DotMultiply == wordType || WT_DotDivide == wordType;
}

bool SubTermNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // term => factor morefactors
    // morefactors => * factor morefactors
    //              | / factor morefactors
    //              | .* factor morefactors
    //              | ./ factor morefactors
    //              | nul

    if (_IsFirstOne) {
        // => factors

        isSuccess = isSuccess && _FactorNT.Parse(exprContextRef, wordCursorRef);
    }
    else {
        // => * factor morefactors
        //  | / factor morefactors
        //  | .* factor morefactors
        //  | ./ factor morefactors

        if (IsInFirstSet(wordCursorRef.CurrentWord().WordType(), false)) {
            int operatorWordIdx = wordCursorRef.CurrentIdx();
            WordTypeEnum operatorWordType = wordCursorRef.CurrentWord().WordType();

            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            isSuccess = isSuccess && _FactorNT.Parse(exprContextRef, wordCursorRef);
            isSuccess = isSuccess && OperatorValidate(exprContextRef, operatorWordType, operatorWordIdx);
        }
        else {
            exprContextRef.SetError("Expect a divide or a multiply operator.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }

    return isSuccess;
}

bool SubTermNT::OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx)
{
    _pExprILCode = FindExprILCode(operWordType, _pLeftOne->ResultType(), _FactorNT.ResultType());

    if (nullptr != _pExprILCode) {
        return true;
    }
    else {
        string errStr = ExprILHelper::GenOperNotFoundErr(operWordType, _pLeftOne->ResultType(), _FactorNT.ResultType());

        exprContextRef.SetError(errStr.c_str(), operWordIdx);

        return false;
    }
}

ResultTypeEnum SubTermNT::GetResultType(void)
{
    if (_IsFirstOne) {
        return _FactorNT.ResultType();
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        return (_pExprILCode->GetReturnType());
    }
}

SubTermNT* SubTermNT::GetLeftSubTerm()
{
    return _pLeftOne;
}

FactorNT* SubTermNT::GetFactor()
{
    return &_FactorNT;
}

ExprILCodeSegment& SubTermNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    if (_IsFirstOne) {
        _FactorNT.AppendILSegment(ilSegment);
    }
    else {
        _ASSERT(NULL != _pLeftOne);
        _ASSERT(nullptr != _pExprILCode);

        _pLeftOne->AppendILSegment(ilSegment);
        _FactorNT.AppendILSegment(ilSegment);
        ilSegment.Append(_pExprILCode->Duplicate());
    }

    return ilSegment;
}
