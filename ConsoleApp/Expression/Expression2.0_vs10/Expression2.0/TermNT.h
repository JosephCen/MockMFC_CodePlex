#ifndef TermNT_H
#define TermNT_H

#include "BaseNonTerminal.h"

//---------------------------------------------------------------------
// TermNT - class
//---------------------------------------------------------------------
class TermNT : public BaseNonTerminal
{
private :
    SubTermNT *_pSubTermNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    TermNT(void);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    SubTermNT* GetSubTerm();
    // Destructor
    virtual ~TermNT();
protected :
    virtual ResultTypeEnum GetResultType();
private :
    // Constructor (Do not allow copy construct)
    TermNT(const TermNT&);
	// Assignment operator (Do not allow assignment operator)
	TermNT& operator=(const TermNT&);
};

#endif