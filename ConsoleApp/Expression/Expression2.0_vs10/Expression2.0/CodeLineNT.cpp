#include "stdafx.h"
#include "NonTerminal.h"
#include "WordParser.h"
#include "ExprILCode.h"
#include "ExprWorkSpace.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - CodeLineNT
//---------------------------------------------------------------------
CodeLineNT::CodeLineNT(void) :
BaseNonTerminal(), _pInnerNT(nullptr)
{ }

CodeLineNT::CodeLineNT(CodeLineNT &&rValRef) :
BaseNonTerminal(move(rValRef)), _pInnerNT(rValRef._pInnerNT)
{
    rValRef._pInnerNT = nullptr;
}

CodeLineNT::~CodeLineNT()
{
	delete _pInnerNT;
	_pInnerNT = nullptr;
}

bool CodeLineNT::IsInFirstSet(WordTypeEnum wordType)
{
    return AssignExprNT::IsInFirstSet(wordType);
}

bool CodeLineNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    // codeline => assign_expr

    bool isSuccess = true;

	_pInnerNT = new AssignExprNT();
    isSuccess = isSuccess && _pInnerNT->Parse(exprContextRef, wordCursorRef);

	return isSuccess;
}

ExprILCodeSegment& CodeLineNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    return _pInnerNT->AppendILSegment(ilSegment);
}

