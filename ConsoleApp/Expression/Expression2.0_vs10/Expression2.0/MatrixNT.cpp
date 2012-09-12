#include "stdafx.h"
#include "MatrixNT.h"
#include "MatrixRowsNT.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - MatrixNT
//---------------------------------------------------------------------
bool MatrixNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(matrix) = { [ }
    return WT_S_Bracket_L == wordType;
}

MatrixNT::MatrixNT(void) : 
_pMatrixRows(nullptr), BaseNonTerminal() 
{
    _pMatrixRows = new MatrixRowsNT();
}

MatrixNT::~MatrixNT()
{
    delete _pMatrixRows;
    _pMatrixRows = nullptr;
}

bool MatrixNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;
    int leftSquareBracketIdx = 0;

    // matrix => [ rows ]
    if (WT_S_Bracket_L == wordCursorRef.CurrentWord().WordType()) {
        leftSquareBracketIdx = wordCursorRef.CurrentIdx();
        isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
    }
    else {
        isSuccess = false;
        exprContextRef.SetError("Unexpected word. Expect a left sqare bracket.", wordCursorRef.CurrentIdx());
    }
    isSuccess = isSuccess && _pMatrixRows->Parse(exprContextRef, wordCursorRef);
    if (isSuccess) {
        if (WT_S_Bracket_R == wordCursorRef.CurrentWord().WordType()) {
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
        }
        else {
            exprContextRef.SetError("Square bracket do not match. Missing the right square bracket.", leftSquareBracketIdx);
            isSuccess = false;
        }
    }

    return isSuccess;
}

Matrix::Row_Col_t MatrixNT::Rows() const
{ 
    return _pMatrixRows->Rows();
}

Matrix::Row_Col_t MatrixNT::Cols() const
{
    return _pMatrixRows->Cols();
}

ResultTypeEnum MatrixNT::GetResultType(void)
{
    return RT_Matrix;
}

ExprILCodeSegment& MatrixNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    _pMatrixRows->AppendILSegment(ilSegment);
    ilSegment.Append(new CtorMatrixILCode(_pMatrixRows->Rows(), _pMatrixRows->Cols()));

    return ilSegment;
}
