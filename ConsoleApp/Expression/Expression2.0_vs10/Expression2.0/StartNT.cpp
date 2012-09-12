#include "stdafx.h"
#include "StartNT.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - StartNT
//---------------------------------------------------------------------
StartNT::StartNT(void) :
_ListNT(), BaseNonTerminal()
{ }

bool StartNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(start) = { ( | Defparam | Num | [ | nul | Eof }
    return WT_Eof == wordType || ListNT::IsInFirstSet(wordType);
}

bool StartNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // start => list Eof

    isSuccess = isSuccess && _ListNT.Parse(exprContextRef, wordCursorRef);
    if (isSuccess)
    {
        if (WT_Eof != wordCursorRef.CurrentWord().WordType())
        {
            exprContextRef.SetError("A unexpected word.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }

    return isSuccess;
}

ExprILCodeSegment& StartNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    return _ListNT.AppendILSegment(ilSegment);
}