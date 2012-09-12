#ifndef CodeLineNT_H
#define CodeLineNT_H

#include "BaseNonTerminal.h"

//---------------------------------------------------------------------
// CodeLineNT - class
//---------------------------------------------------------------------
class CodeLineNT : public BaseNonTerminal
{
private :
	BaseNonTerminal *_pInnerNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    CodeLineNT(void);
    CodeLineNT(CodeLineNT &&rValRef);
    // Destructor
    virtual ~CodeLineNT();
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
private :
    // Constructor (Do not allow copy construct)
    CodeLineNT(const CodeLineNT&);
    // Assignment operator (Do not allow assignment operator)
    CodeLineNT& operator=(const CodeLineNT&);
};

#endif