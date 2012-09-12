#include "stdafx.h"
#include "TermNT.h"
#include "FactorNT.h"
#include "SubTermNT.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - TermNT
//---------------------------------------------------------------------
TermNT::TermNT(void) :
_pSubTermNT(NULL), BaseNonTerminal()
{ }

TermNT::~TermNT()
{
    if (NULL != _pSubTermNT) {
        delete _pSubTermNT;
        _pSubTermNT = NULL;
    }
}

bool TermNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(term) = { ( | Defparam | Num | [ }
    return FactorNT::IsInFirstSet(wordType);
}

bool TermNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;
    //ResultTypeEnum resultType_L;
    //ResultTypeEnum resultType_R;

    // term => factor morefactors
    // morefactors => * factor morefactors
    //              | / factor morefactors
    //              | .* factor morefactors
    //              | ./ factor morefactors
    //              | nul

    _pSubTermNT = new SubTermNT();
    isSuccess = isSuccess && _pSubTermNT->Parse(exprContextRef, wordCursorRef);

    while (isSuccess && SubTermNT::IsInFirstSet(wordCursorRef.CurrentWord().WordType(), false)) {
        _pSubTermNT = new SubTermNT(_pSubTermNT);
        isSuccess = isSuccess && _pSubTermNT->Parse(exprContextRef, wordCursorRef);
    }

    return isSuccess;
}

ResultTypeEnum TermNT::GetResultType(void)
{
    _ASSERT(NULL != _pSubTermNT);
    return _pSubTermNT->ResultType();
}

SubTermNT* TermNT::GetSubTerm()
{
    return _pSubTermNT;
}

ExprILCodeSegment& TermNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    _ASSERT(NULL != _pSubTermNT);

    return _pSubTermNT->AppendILSegment(ilSegment);
}
