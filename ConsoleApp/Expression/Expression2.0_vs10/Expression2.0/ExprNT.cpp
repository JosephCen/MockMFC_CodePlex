#include "stdafx.h"
#include "ExprNT.h"
#include "SubExprNT.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - ExprNT
//---------------------------------------------------------------------
ExprNT::ExprNT(void) : 
_pSubExprNT(NULL), BaseNonTerminal()
{ }

ExprNT::~ExprNT()
{
    if (NULL != _pSubExprNT) {
        delete _pSubExprNT;
        _pSubExprNT = NULL;
    }
}

bool ExprNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(expr) = { ( | Defparam | Num | [ }
    return SubExprNT::IsInFirstSet(wordType, true);
}

bool ExprNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;
    //ResultTypeEnum resultType_L;
    //ResultTypeEnum resultType_R;

    // expr => term moreterms
    // moreterms => + term moreterms
    //            | - term moreterms
    //            | nul

    _pSubExprNT = new SubExprNT();
    isSuccess = isSuccess && _pSubExprNT->Parse(exprContextRef, wordCursorRef);

    while (isSuccess && SubExprNT::IsInFirstSet(wordCursorRef.CurrentWord().WordType(), false)) {
        _pSubExprNT = new SubExprNT(_pSubExprNT);
        isSuccess = isSuccess && _pSubExprNT->Parse(exprContextRef, wordCursorRef);
    }

    return isSuccess;
}

ResultTypeEnum ExprNT::GetResultType(void)
{
    _ASSERT(NULL != _pSubExprNT);
    
    return _pSubExprNT->ResultType();
}

SubExprNT* ExprNT::GetSubExpr()
{
    return _pSubExprNT;
}

ExprILCodeSegment& ExprNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    _ASSERT(NULL != _pSubExprNT);

    return _pSubExprNT->AppendILSegment(ilSegment);
}
