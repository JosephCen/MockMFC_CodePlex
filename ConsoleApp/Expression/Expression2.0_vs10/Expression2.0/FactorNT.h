#ifndef FactorNT_H
#define FactorNT_H

#include "BaseNonTerminal.h"

//---------------------------------------------------------------------
// FactorNT - class
//---------------------------------------------------------------------
class FactorNT : public BaseNonTerminal
{
private :
    int _Flag;
	BaseNonTerminal *_pInnerNT;
    ExprILCode *_pExprILCode;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    FactorNT(void);
	// Destructor
    virtual ~FactorNT();
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    MatrixNT* GetInnerMatrix();
    FunctionNT* GetInnerFunction();
    AssignExprNT* GetSubAssignExpr();
    bool GetIsDefVar();
    bool GetIsRealVal();
protected :
    virtual ResultTypeEnum GetResultType(void);
private :
    // Constructor (Do not allow copy construct)
    FactorNT(const FactorNT&);
	// Assignment operator (Do not allow assignment operator)
	FactorNT& operator=(const FactorNT&);
};

#endif