#ifndef MatrixColNT_H
#define MatrixColNT_H

#include "BaseNonTerminal.h"
#include "ExprNT.h"

//---------------------------------------------------------------------
// MatrixColNT - class
//---------------------------------------------------------------------
class MatrixColNT : public BaseNonTerminal
{
private :
    ExprNT _ExprNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    MatrixColNT(void);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
protected :
    virtual ResultTypeEnum GetResultType(void) { return _ExprNT.ResultType(); }
};

#endif