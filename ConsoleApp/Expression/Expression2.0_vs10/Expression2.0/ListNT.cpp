#include "stdafx.h"
#include "ListNT.h"
#include "WordParser.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - ListNT
//---------------------------------------------------------------------
ListNT::ListNT(void) :
_IsEndWithSemicolon(true), _CodeList(), BaseNonTerminal()
{ }

bool ListNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(list) = { ( | Defparam | Num | [ | nul }
    return WT_Nul == wordType || CodeLineNT::IsInFirstSet(wordType);
}

bool ListNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // list => codeline ; list
    //       | nul

    while (isSuccess && CodeLineNT::IsInFirstSet(wordCursorRef.CurrentWord().WordType())) {
        CodeLineNT curCodeLine;

        isSuccess = isSuccess && curCodeLine.Parse(exprContextRef, wordCursorRef);
        if (isSuccess) {
            _CodeList.push_back(move(curCodeLine));
            if (WT_Semicolon == wordCursorRef.CurrentWord().WordType()) {
                _IsEndWithSemicolon = true;
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            }
            else {
                _IsEndWithSemicolon = false;
                break;
            }
        }
    }

    return isSuccess;
}

ExprILCodeSegment& ListNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    for (auto iter = _CodeList.begin(); _CodeList.end() != iter; ++iter) {
        iter->AppendILSegment(ilSegment);

        //if (_IsEndWithSemicolon || _ExprList.back() != *iter) {
        //    // TODO: Remove run stack top node.
        //}
    }

    return ilSegment;
}