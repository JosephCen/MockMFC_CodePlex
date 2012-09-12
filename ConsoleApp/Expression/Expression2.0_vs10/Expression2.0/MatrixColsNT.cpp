#include "stdafx.h"
#include "MatrixColsNT.h"
#include "MatrixColNT.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - MatrixColsNT
//---------------------------------------------------------------------
MatrixColsNT::MatrixColsNT(void) :
_ColList(), BaseNonTerminal()
{ }

MatrixColsNT::~MatrixColsNT()
{
    for (ColList_t::iterator iter = _ColList.begin(); _ColList.end() != iter; ++iter) {
        delete *iter;
        *iter = NULL;
    }
}

bool MatrixColsNT::IsInFirstSet(WordTypeEnum wordType)
{
    // cols => col
    //       | col , cols
    return MatrixColNT::IsInFirstSet(wordType);
}

bool MatrixColsNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    _ASSERT(IsInFirstSet(wordCursorRef.CurrentWord().WordType()));

    bool isSuccess = true;

    // cols => col
    //       | col , cols

    do {
        _ColList.push_back(new MatrixColNT());
        isSuccess = isSuccess && _ColList.back()->Parse(exprContextRef, wordCursorRef);
        if (isSuccess && WT_Comma == wordCursorRef.CurrentWord().WordType())
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
        else
            break;
    } while (isSuccess);

    return isSuccess;
}

ExprILCodeSegment& MatrixColsNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    for (ColList_t::iterator iter = _ColList.begin(); _ColList.end() != iter; ++iter)
        (*iter)->AppendILSegment(ilSegment);

    return ilSegment;
}
