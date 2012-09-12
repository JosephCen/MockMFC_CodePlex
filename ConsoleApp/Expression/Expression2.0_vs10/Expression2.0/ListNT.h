#ifndef ListNT_H
#define ListNT_H

#include "BaseNonTerminal.h"
#include "CodeLineNT.h"

//---------------------------------------------------------------------
// ListNT - class
//---------------------------------------------------------------------
class ListNT : public BaseNonTerminal
{
private :
    typedef std::vector<CodeLineNT> CodeList_t;
    CodeList_t _CodeList;
    bool _IsEndWithSemicolon;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    ListNT(void);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
private :
    // Constructor (Do not allow copy construct)
    ListNT(const ListNT&);
    // Assignment operator (Do not allow assignment operator)
    ListNT& operator=(const ListNT&);
};

#endif