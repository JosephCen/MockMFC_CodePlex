#include "stdafx.h"
#include "ExprWorkSpace.h"
#include "VariableSet.h"
#include "ExprAdapter.h"
#include "WordParser.h"
#include "ExprILCodeSegment.h"
#include "NonTerminal.h"
#include <string>
using std::string;

//---------------------------------------------------------------------
// Class member - ExprWorkSpace
//---------------------------------------------------------------------
ExprWorkSpace::ExprWorkSpace() :
_GlobalVarSet(), _WordParser()
{ }

bool ExprWorkSpace::ParseILCodeSegment(const string &codeStr, ExprILCodeSegment *pILSegment)
{
    WordFwCursor wordCursor = _WordParser.GenWordFwCursor(codeStr);
    ExprContext exprContext;

    if (wordCursor.NextWord(exprContext)) {
        StartNT startNT;

        if (startNT.Parse(exprContext, wordCursor)) {
            startNT.AppendILSegment(*pILSegment);

            return true;
        }
    }

    return false;
}