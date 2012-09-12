#ifndef ExprNT_H
#define ExprNT_H

#include "BaseNonTerminal.h"

//---------------------------------------------------------------------
// ExprNT - class
//---------------------------------------------------------------------
class ExprNT : public BaseNonTerminal
{
private :
    SubExprNT *_pSubExprNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    ExprNT(void);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    SubExprNT* GetSubExpr();
    // Destructor
    virtual ~ExprNT();
protected :
    virtual ResultTypeEnum GetResultType(void);
private :
    // Constructor (Do not allow copy construct)
    ExprNT(const ExprNT&);
	// Assignment operator (Do not allow assignment operator)
	ExprNT& operator=(const ExprNT&);
};

#endif