#include "stdafx.h"
#include "ExprAdapter.h"
#include <string>
using std::string;
#include "WordParser.h"
#include "NonTerminal.h"
#include <crtdbg.h>

// Member of class 'ExprContext'

ExprContext::~ExprContext()
{
    if (NULL != _pExprEx) {
        delete _pExprEx;
        _pExprEx = NULL;
    }
}

void ExprContext::SetError(const ExprException &exprExRef)
{
    _ASSERT(NULL == _pExprEx);

    _pExprEx = new ExprException(exprExRef);
}

void ExprContext::SetError(const char *errorCh, int chIdx)
{
    _ASSERT(NULL == _pExprEx);

    _pExprEx = new ExprException(errorCh, chIdx);
}

// Member of class 'ExprContext'

ExprAdapter::~ExprAdapter()
{
}

bool ExprAdapter::Parse(const string &exprStr)
{
    _ASSERT(false == _IsParsed);
    
    _IsParsed = true;
    WordParser wordParser;
    StartNT startNT;

    WordFwCursor wordFwCursor = wordParser.GenWordFwCursor(exprStr);
    // TODO: !!!
    //if (wordParser.NextWord(_CurContext)) {
    //    
    //    /*if (startNT.Parse(_CurContext, wordParser)) {
    //        startNT.AppendILSegment(_ILSegment);
    //    }*/
    //}

    return (bool)_CurContext;
}