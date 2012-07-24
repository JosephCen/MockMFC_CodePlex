#include "stdafx.h"
#include "NonTerminal.h"
#include "WordParser.h"
#include "ExprILCode.h"
#include "ExprWorkSpace.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - AssignExprNT
//---------------------------------------------------------------------
AssignExprNT::AssignExprNT(void) :
BaseNonTerminal(), _Flag(F_None), _spExprILCode(), _Expr(), _pRightOne(nullptr)
{ }

AssignExprNT::~AssignExprNT()
{
    delete _pRightOne;
    _pRightOne = nullptr;
}

bool AssignExprNT::IsInFirstSet(WordTypeEnum wordType)
{
    return WT_UndefFuncVal == wordType || ExprNT::IsInFirstSet(wordType);
}

bool AssignExprNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    // assign_expr => undeffuncval = assign_expr
    //              | expr = assign_expr
    //              | expr

    bool isSuccess = true;

    WordTypeEnum wordType = wordCursorRef.CurrentWord().WordType();
    if (WT_UndefFuncVal == wordType) {
        // assign_expr => undeffuncval = assign_expr

        int undefVarIdx = wordCursorRef.CurrentIdx();

        _spExprILCode.reset(new NewVariableILCode(wordCursorRef.CurrentWord().StringValue()));
        isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
        if (isSuccess && WT_Assignment == wordCursorRef.CurrentWord().WordType()) {
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            _Flag = F_UnDefVar;
            _pRightOne = new AssignExprNT();
            isSuccess = isSuccess && _pRightOne->Parse(exprContextRef, wordCursorRef);
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
                // TODO: Check the _Expr is a left value
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                _Flag = F_DefinedVar;
                _pRightOne = new AssignExprNT();
                isSuccess = isSuccess && _pRightOne->Parse(exprContextRef, wordCursorRef);
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
        ilSegment.Append(_spExprILCode);
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
