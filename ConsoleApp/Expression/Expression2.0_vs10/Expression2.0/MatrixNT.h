#ifndef MatrixNT_H
#define MatrixNT_H

#include "BaseNonTerminal.h"

//---------------------------------------------------------------------
// MatrixNT - class
//---------------------------------------------------------------------
class MatrixNT : public BaseNonTerminal
{
private :
    MatrixRowsNT *_pMatrixRows;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    MatrixNT(void);
    virtual ~MatrixNT();
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    Matrix::Row_Col_t Rows() const;
    Matrix::Row_Col_t Cols() const;
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
protected :
    virtual ResultTypeEnum GetResultType(void);
};

#endif