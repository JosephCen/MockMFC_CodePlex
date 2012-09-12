#include "stdafx.h"
#include "MatrixRowsNT.h"
#include "MatrixColsNT.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - MatrixRowsNT
//---------------------------------------------------------------------
MatrixRowsNT::MatrixRowsNT(void) : 
_RowList(), BaseNonTerminal()
{ }

MatrixRowsNT::~MatrixRowsNT()
{
    for (RowList_t::iterator iter = _RowList.begin(); _RowList.end() != iter; ++iter) {
        delete *iter;
        *iter = NULL;
    }
}

bool MatrixRowsNT::IsInFirstSet(WordTypeEnum wordType)
{
    // rows => cols
    //       | cols ; rows
    return MatrixColsNT::IsInFirstSet(wordType);
}

bool MatrixRowsNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    _ASSERT(IsInFirstSet(wordCursorRef.CurrentWord().WordType()));

    bool isSuccess = true;
    Matrix::Row_Col_t cols = 0;

    // rows => cols
    //       | cols ; rows
    do {
        _RowList.push_back(new MatrixColsNT());
        if (_RowList.back()->Parse(exprContextRef, wordCursorRef)) {
            cols = (0 == cols ? _RowList.back()->Cols() : cols);
            if (cols != _RowList.back()->Cols()) {
                exprContextRef.SetError("Cols in a matrix could not be different.", wordCursorRef.CurrentIdx());
                isSuccess = false;
            }
        }
        else {
            isSuccess = false;
        }
        if (isSuccess && WT_Semicolon == wordCursorRef.CurrentWord().WordType())
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
        else
            break;
    } while (isSuccess);

    return isSuccess;
}

Matrix::Row_Col_t MatrixRowsNT::Cols() const
{
    return _RowList.front()->Cols();
}

ExprILCodeSegment& MatrixRowsNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    for (RowList_t::iterator iter = _RowList.begin(); _RowList.end() != iter; ++iter)
        (*iter)->AppendILSegment(ilSegment);

    return ilSegment;
}
