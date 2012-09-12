#include "stdafx.h"
#include "LeftValueInterpreter.h"
#include "AssignExprNT.h"
#include "ExprNT.h"
#include "SubExprNT.h"
#include "TermNT.h"
#include "SubTermNT.h"
#include "FactorNT.h"
#include <crtdbg.h>
using namespace std;

#define CHECK_ISDEFINITE() _ASSERT_EXPR(!GetIsDefinite(), \
    L"Can not call method [Interpret] against an instance which has got a definited value.")

LeftValueInterpreter::LeftValueInterpreter(void) :
    _IsDefinite(false), _IsLeftValue(false)
{ }

void LeftValueInterpreter::Interpret(StartNT *pStartNT)
{
    _ASSERT_EXPR(0, L"LeftValueInterpreter cannot be called against [StartNT].");
}

void LeftValueInterpreter::Interpret(ListNT *pListNT)
{
    _ASSERT_EXPR(0, L"LeftValueInterpreter cannot be called against [ListNT].");
}

void LeftValueInterpreter::Interpret(CodeLineNT *pCodeLineNT)
{
    _ASSERT_EXPR(0, L"LeftValueInterpreter cannot be called against [CodeLineNT].");
}

void LeftValueInterpreter::Interpret(AssignExprNT *pAssignExprNT)
{
    CHECK_ISDEFINITE();
    _ASSERT(nullptr != pAssignExprNT);
    
    ExprNT *pExpr = pAssignExprNT->GetSubExpr();
    
    if (nullptr != pExpr) {
        Interpret(pExpr);
    }
    else {
        if (pAssignExprNT->GetIsDefinedVar() || pAssignExprNT->GetIsUnDefinedVar())
            SetIsLeftValue(false);
    }
}

void LeftValueInterpreter::Interpret(ExprNT *pExprNT)
{
    CHECK_ISDEFINITE();
    _ASSERT(nullptr != pExprNT);

    SubExprNT *pSubExprNT = pExprNT->GetSubExpr();

    if (nullptr != pSubExprNT)
        Interpret(pSubExprNT);
}

void LeftValueInterpreter::Interpret(SubExprNT *pSubExprNT)
{
    CHECK_ISDEFINITE();
    _ASSERT(nullptr != pSubExprNT);

    if (nullptr != pSubExprNT->GetLeftSubExpr()) {
        SetIsLeftValue(false);
    }
    else {
        TermNT *pTermNT = pSubExprNT->GetTerm();

        if (nullptr != pTermNT)
            Interpret(pTermNT);
    }
}

void LeftValueInterpreter::Interpret(TermNT *pTermNT)
{
    CHECK_ISDEFINITE();
    _ASSERT(nullptr != pTermNT);

    SubTermNT *pSubTermNT = pTermNT->GetSubTerm();
    if (nullptr != pSubTermNT)
        Interpret(pSubTermNT);
}

void LeftValueInterpreter::Interpret(SubTermNT *pSubTermNT)
{
    CHECK_ISDEFINITE();
    _ASSERT(nullptr != pSubTermNT);

    if (nullptr != pSubTermNT->GetLeftSubTerm()) {
        SetIsLeftValue(false);
    }
    else {
        FactorNT *pFactorNT = pSubTermNT->GetFactor();

        if (nullptr != pFactorNT)
            Interpret(pFactorNT);
    }
}

void LeftValueInterpreter::Interpret(FactorNT *pFactorNT)
{
    CHECK_ISDEFINITE();
    _ASSERT(nullptr != pFactorNT);

    if (pFactorNT->GetIsDefVar()) {
        SetIsLeftValue(true);
    }
    else {
        AssignExprNT *pAssignExprNT = pFactorNT->GetSubAssignExpr();

        if (nullptr != pAssignExprNT) {
            Interpret(pAssignExprNT);
        }
        else {
            if (pFactorNT->GetIsRealVal() || nullptr != pFactorNT->GetInnerFunction() || nullptr != pFactorNT->GetInnerMatrix())
                SetIsLeftValue(false);
        }
    }
}

void LeftValueInterpreter::Interpret(MatrixNT *pMatrixNT)
{
    _ASSERT_EXPR(0, L"LeftValueInterpreter cannot be called against [MatrixNT].");
}

void LeftValueInterpreter::Interpret(FunctionNT *pFunctionNT)
{
    _ASSERT_EXPR(0, L"LeftValueInterpreter cannot be called against [FunctionNT].");
}

bool LeftValueInterpreter::GetIsLeftValue() const
{
    _ASSERT_EXPR(_IsDefinite, L"Value must have been defined before access it");

    return _IsLeftValue;
}

bool LeftValueInterpreter::GetIsDefinite() const
{
    return _IsDefinite;
}

void LeftValueInterpreter::SetIsLeftValue(bool isLeftValue)
{
    _IsDefinite = true;
    _IsLeftValue = isLeftValue;
}