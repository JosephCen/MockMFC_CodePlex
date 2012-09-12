#ifndef AssignExprNT_H
#define AssignExprNT_H

#include "BaseNonTerminal.h"
#include "ExprNT.h"

//---------------------------------------------------------------------
// AssignExprNT - class
//---------------------------------------------------------------------
class AssignExprNT : public BaseNonTerminal
{
private :
    int _Flag;
    ExprILCode *_pExprILCode;
    ExprNT _Expr;
    AssignExprNT *_pRightOne;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    AssignExprNT(void);
    // Destructor
    virtual ~AssignExprNT();
	// Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    ExprNT* GetSubExpr();
    bool GetIsDefinedVar();
    bool GetIsUnDefinedVar();
protected :
    virtual ResultTypeEnum GetResultType();
private :
    // Constructor (Do not allow copy construct)
    AssignExprNT(const AssignExprNT&);
	// Assignment operator (Do not allow assignment operator)
	AssignExprNT& operator=(const AssignExprNT&);
};

#endif