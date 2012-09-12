#include "stdafx.h"
#include "MatrixColNT.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - MatrixColNT
//---------------------------------------------------------------------
MatrixColNT::MatrixColNT(void) :
_ExprNT(), BaseNonTerminal()
{ }

bool MatrixColNT::IsInFirstSet(WordTypeEnum wordType)
{
    // col => expr
    return ExprNT::IsInFirstSet(wordType);
}

bool MatrixColNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // col => expr

    if (IsInFirstSet(wordCursorRef.CurrentWord().WordType())) {
        int exprStartIdx = wordCursorRef.CurrentIdx();
        isSuccess = isSuccess && _ExprNT.Parse(exprContextRef, wordCursorRef);
        if (isSuccess && RT_String == _ExprNT.ResultType()) {
            isSuccess = false;
            exprContextRef.SetError("Just real value or matrix could be contain in a matrix", exprStartIdx);
        }
    }
    else {
        isSuccess = false;
        exprContextRef.SetError("Unexpected word. Expect a real value or an sub expression.", wordCursorRef.CurrentIdx());
    }

    return isSuccess;
}

ExprILCodeSegment& MatrixColNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    return _ExprNT.AppendILSegment(ilSegment);
}
