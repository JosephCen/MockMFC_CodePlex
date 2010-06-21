#include "stdafx.h"
#include "ExprWorkSpace.h"
#include "VariableSet.h"
#include "ExprAdapter.h"
#include "WordParser.h"
#include "ExprILCodeSegment.h"
#include "NonTerminal.h"
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <crtdbg.h>

//---------------------------------------------------------------------
// Class member - ExprWorkSpace
//---------------------------------------------------------------------
ExprWorkSpace::ExprWorkSpace() :
_GlobalVarSet(), _WordParser(), _ILCodeSegmentSet()
{ }

ExprWorkSpace::~ExprWorkSpace()
{
    for (ILCodeSegmentIter_t iter = _ILCodeSegmentSet.begin(); _ILCodeSegmentSet.end() != iter; ++iter)
        delete *iter;
    _ILCodeSegmentSet.clear();
}

bool ExprWorkSpace::ParseILCodeSegment(const std::string &codeStr, ExprILCodeSegment **ppILSegment)
{
    _ASSERT(NULL != ppILSegment);

    WordFwCursor wordCursor = _WordParser.GenWordFwCursor(codeStr);
    ExprContext exprContext;

    if (wordCursor.NextWord(exprContext)) {
        StartNT startNT;

        if (startNT.Parse(exprContext, wordCursor)) {
            ExprILCodeSegment *pILSegment = new ExprILCodeSegment();

            _ILCodeSegmentSet.push_back(pILSegment);
            startNT.AppendILSegment(*pILSegment);
            *ppILSegment = pILSegment;

            return true;
        }
    }

    return false;
}

bool ExprWorkSpace::RunILCodeSegment(ExprILCodeSegment *pILSegment, Variable **ppVariable)
{
    _ASSERT(NULL != pILSegment);
    _ASSERT(NULL != ppVariable);

    return (pILSegment->Run(&_GlobalVarSet, ppVariable));
}