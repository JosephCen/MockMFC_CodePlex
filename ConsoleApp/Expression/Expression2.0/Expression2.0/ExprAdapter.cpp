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