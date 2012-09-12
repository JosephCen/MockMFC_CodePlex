#ifndef SubTermNT_H
#define SubTermNT_H

#include "BaseNonTerminal.h"
#include "FactorNT.h"

//---------------------------------------------------------------------
// SubTermNT - class
//---------------------------------------------------------------------
class SubTermNT : public BaseNonTerminal
{
private :
    bool _IsFirstOne;
    ExprILCode *_pExprILCode;
    FactorNT _FactorNT;
    SubTermNT *_pLeftOne;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType, bool isFirstOne);
    // Constructor
    SubTermNT(void);
    SubTermNT(SubTermNT *pLeftOne);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    SubTermNT* GetLeftSubTerm();
    FactorNT* GetFactor();
    // Destructor
    virtual ~SubTermNT();
private :
    bool OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx);
protected :
    virtual ResultTypeEnum GetResultType(void);
private :
    // Constructor (Do not allow copy construct)
    SubTermNT(const SubTermNT&);
	// Assignment operator (Do not allow assignment operator)
	SubTermNT& operator=(const SubTermNT&);
};

#endif