#ifndef StartNT_H
#define StartNT_H

#include "BaseNonTerminal.h"
#include "ListNT.h"

//---------------------------------------------------------------------
// StartNT - class
//---------------------------------------------------------------------
class StartNT : public BaseNonTerminal
{
private :
    ListNT _ListNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    StartNT(void);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
};

#endif