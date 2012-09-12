#include "stdafx.h"
#include "FactorNT.h"
#include "AssignExprNT.h"
#include "FunctionNT.h"
#include "MatrixNT.h"
#include "WordParser.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - FactorNT
//---------------------------------------------------------------------
enum Flag
{
    F_None,
    F_Matrix,
    F_Func,
    F_SubAssignExpr,
    F_DefVar,
    F_RealVal
};

FactorNT::FactorNT(void): 
    BaseNonTerminal(), _pInnerNT(nullptr), _pExprILCode(nullptr),
    _Flag(F_None)
{ }

FactorNT::~FactorNT()
{
    delete _pExprILCode;
    _pExprILCode = nullptr;
    delete _pInnerNT; // Call delete against a null pointer is harmless.
    _pInnerNT = nullptr;
}

bool FactorNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(factor) = { ( | Defparam | + | - | Num | [ | Deffunc}
    return WT_Paranthese_L == wordType || WT_DefVariable == wordType 
           || WT_RealValue == wordType || WT_Plus == wordType || WT_Minus == wordType
           || MatrixNT::IsInFirstSet(wordType) || FunctionNT::IsInFirstSet(wordType);
}

bool FactorNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // factor => ( expr )
    //         | Defparam
    //         | Num
    //         | + Num
    //         | - Num
    //         | matrix

    WordTypeEnum wordType = wordCursorRef.CurrentWord().WordType();
    if (MatrixNT::IsInFirstSet(wordType)) {
        // factor => matrix

        _pInnerNT = new MatrixNT();
        isSuccess = isSuccess && _pInnerNT->Parse(exprContextRef, wordCursorRef);
        if (isSuccess)
            _Flag = F_Matrix;
    }
    else if (FunctionNT::IsInFirstSet(wordType)) {
        // factor => Deffunc

        _pInnerNT = new FunctionNT();
        isSuccess = isSuccess && _pInnerNT->Parse(exprContextRef, wordCursorRef);
        if (isSuccess)
            _Flag = F_Func;
    }
    else {
        int leftParantheseIdx = 0;
        int plusMinusIdx = 0;

        switch (wordType) {
            case WT_Paranthese_L :
                // factor => ( expr )

                leftParantheseIdx = wordCursorRef.CurrentIdx();
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                _pInnerNT = new AssignExprNT();
                isSuccess = isSuccess && _pInnerNT->Parse(exprContextRef, wordCursorRef);
                if (isSuccess) {
                    if (WT_Paranthese_R == wordCursorRef.CurrentWord().WordType()) {
                        isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                    }
                    else {
                        isSuccess = false;
                        exprContextRef.SetError("Paranthese do not match. Missing the right paranthese.", leftParantheseIdx);
                    }
                }
                if (isSuccess)
                    _Flag = F_SubAssignExpr;
                break;
            case WT_DefVariable :
                // factor => Defparam

                _pExprILCode = new PushDefValILCode(wordCursorRef.CurrentWord().StringValue(),
                                        static_cast<ResultTypeEnum>(wordCursorRef.CurrentWord().IntValue()));
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                if (isSuccess)
                    _Flag = F_DefVar;
                break;
            case WT_RealValue :
                // factor => Num

                _pExprILCode = new PushRealValILCode(wordCursorRef.CurrentWord().RealValue());
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                if (isSuccess)
                    _Flag = F_RealVal;
                break;
            case WT_Plus :
            case WT_Minus :
                plusMinusIdx = wordCursorRef.CurrentIdx();
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                if (isSuccess) {
                    if (WT_RealValue == wordCursorRef.CurrentWord().WordType()) {
                        _pExprILCode = new PushRealValILCode(
                            WT_Minus == wordType ? (-1.0 * wordCursorRef.CurrentWord().RealValue()) : wordCursorRef.CurrentWord().RealValue());
                        isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                    }
                    else {
                        isSuccess = false;
                        exprContextRef.SetError("A real value is expected after a - or +.", plusMinusIdx);
                    }
                }
                if (isSuccess)
                    _Flag = F_RealVal;
                break;
            default :
                isSuccess = false;
                exprContextRef.SetError("Unexpected word. Expect a sub expression or a matrix.", wordCursorRef.CurrentIdx());
                break;
        }
    }

    return isSuccess;
}

ResultTypeEnum FactorNT::GetResultType(void)
{
    if (nullptr != _pInnerNT)
        return _pInnerNT->ResultType();
    else
        return _pExprILCode->GetReturnType();
}

ExprILCodeSegment& FactorNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    if (nullptr != _pInnerNT) {
        _pInnerNT->AppendILSegment(ilSegment);
    }
    else {
        _ASSERT(nullptr != _pExprILCode);

        ilSegment.Append(_pExprILCode->Duplicate());
    }
    
    return ilSegment;
}

MatrixNT* FactorNT::GetInnerMatrix()
{
    return (F_Matrix == _Flag ? (MatrixNT*)_pInnerNT : nullptr);
}

FunctionNT* FactorNT::GetInnerFunction()
{
    return (F_Func == _Flag ? (FunctionNT*)_pInnerNT : nullptr);
}

AssignExprNT* FactorNT::GetSubAssignExpr()
{
    return (F_SubAssignExpr == _Flag ? (AssignExprNT*)_pInnerNT : nullptr);
}

bool FactorNT::GetIsDefVar()
{
    return (F_DefVar == _Flag);
}

bool FactorNT::GetIsRealVal()
{
    return (F_RealVal == _Flag);
}