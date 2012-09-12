#ifndef SubExprNT_H
#define SubExprNT_H

#include "BaseNonTerminal.h"
#include "TermNT.h"

//---------------------------------------------------------------------
// SubExprNT - class
//---------------------------------------------------------------------
class SubExprNT : public BaseNonTerminal
{
private :
    bool _IsFirstOne;
    const ExprILCode *_pExprILCode;
    TermNT _TermNT;
    SubExprNT *_pLeftOne;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType, bool isFirstOne);
    // Constructor
    SubExprNT(void);
    SubExprNT(SubExprNT *pLeftOne);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    SubExprNT* GetLeftSubExpr();
    TermNT* GetTerm();
    // Destructor
    virtual ~SubExprNT();
private :
    bool OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx);
protected :
    virtual ResultTypeEnum GetResultType(void);
private :
    // Constructor (Do not allow copy construct)
    SubExprNT(const SubExprNT&);
	// Assignment operator (Do not allow assignment operator)
	SubExprNT& operator=(const SubExprNT&);
};

#endif
