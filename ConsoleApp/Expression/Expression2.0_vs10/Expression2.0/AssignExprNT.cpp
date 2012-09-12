#include "stdafx.h"
#include "AssignExprNT.h"
#include "WordParser.h"
#include "LeftValueInterpreter.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - AssignExprNT
//---------------------------------------------------------------------
enum Flag
{
    F_None,
    F_JustExpr,
    F_UnDefVar,
    F_DefinedVar,
};

AssignExprNT::AssignExprNT(void) :
BaseNonTerminal(), _Flag(F_None), _pExprILCode(nullptr), _Expr(), _pRightOne(nullptr)
{ }

AssignExprNT::~AssignExprNT()
{
    delete _pExprILCode;
    _pExprILCode = nullptr;
    delete _pRightOne;
    _pRightOne = nullptr;
}

bool AssignExprNT::IsInFirstSet(WordTypeEnum wordType)
{
    return WT_UndefVariable == wordType || ExprNT::IsInFirstSet(wordType);
}

bool AssignExprNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    // assign_expr => undeffuncval = assign_expr
    //              | expr = assign_expr
    //              | expr

    bool isSuccess = true;

    WordTypeEnum wordType = wordCursorRef.CurrentWord().WordType();
    if (WT_UndefVariable == wordType) {
        // assign_expr => undeffuncval = assign_expr

        int undefVarIdx = wordCursorRef.CurrentIdx();

        _pExprILCode = new NewVariableILCode(wordCursorRef.CurrentWord().StringValue());
        isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
        if (isSuccess && WT_Assignment == wordCursorRef.CurrentWord().WordType()) {
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            _pRightOne = new AssignExprNT();
            isSuccess = isSuccess && _pRightOne->Parse(exprContextRef, wordCursorRef);
            if (isSuccess)
                _Flag = F_UnDefVar;
        }
        else {
            isSuccess = false;
            exprContextRef.SetError("Assignment expression (=...) is expected following a undefined variable.", undefVarIdx);
        }
    }
    else {
        // assign_expr => expr = assign_expr
        //              | expr

        isSuccess = isSuccess && _Expr.Parse(exprContextRef, wordCursorRef);
        if (isSuccess) {
            if (WT_Assignment == wordCursorRef.CurrentWord().WordType()) {
                int assignWordIdx = wordCursorRef.CurrentIdx();

                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                if (isSuccess) {
                    LeftValueInterpreter lValInterpreter;

                    lValInterpreter.Interpret(&_Expr);
                    if (lValInterpreter.GetIsDefinite() && lValInterpreter.GetIsLeftValue()) {
                        _pRightOne = new AssignExprNT();
                        isSuccess = isSuccess && _pRightOne->Parse(exprContextRef, wordCursorRef);
                    }
                    else {
                        isSuccess = false;
                        exprContextRef.SetError("Left side of Assignment expression (=) must be a left value.", assignWordIdx);
                    }
                }
                if (isSuccess)
                    _Flag = F_DefinedVar;
            }
            else {
                _Flag = F_JustExpr;
            }
        }
    }

    return isSuccess;
}

ResultTypeEnum AssignExprNT::GetResultType(void)
{
    if (F_JustExpr == _Flag)
        return _Expr.ResultType();
    else
        return _pRightOne->ResultType();
}

ExprILCodeSegment& AssignExprNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    switch (_Flag) {
    case F_JustExpr :
        _Expr.AppendILSegment(ilSegment);
        break;
    case F_UnDefVar :
        _pRightOne->AppendILSegment(ilSegment);
        ilSegment.Append(_pExprILCode->Duplicate());
        break;
    case F_DefinedVar :
        _ASSERT_EXPR(0, L"Assign an existing variable has not been finished."); // TODO
        break;
    default :
        _ASSERT_EXPR(0, L"Unexpected condition");
        break;
    }
    
    return ilSegment;
}

ExprNT* AssignExprNT::GetSubExpr()
{
    return (F_JustExpr == _Flag ? &_Expr : nullptr);
}

bool AssignExprNT::GetIsDefinedVar()
{
    return (F_DefinedVar == _Flag);
}

bool AssignExprNT::GetIsUnDefinedVar()
{
    return (F_UnDefVar == _Flag);
}