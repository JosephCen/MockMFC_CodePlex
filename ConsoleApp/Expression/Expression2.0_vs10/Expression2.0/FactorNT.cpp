#include "stdafx.h"
#include "NonTerminal.h"
#include "WordParser.h"
#include "ExprILCode.h"
#include "ExprWorkSpace.h"
#include "ExprILHelper.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - FactorNT
//---------------------------------------------------------------------
FactorNT::FactorNT(void): 
BaseNonTerminal(), _pInnerNT(nullptr), _spExprILCode() 
{ }

FactorNT::~FactorNT()
{
    delete _pInnerNT; // Call delete against a null pointer is harmless.
    _pInnerNT = nullptr;
}

bool FactorNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(factor) = { ( | Defparam | Num | [ | Deffunc}
    return WT_Paranthese_L == wordType || WT_DefVariable == wordType || WT_RealValue == wordType 
           || MatrixNT::IsInFirstSet(wordType) || FunctionNT::IsInFirstSet(wordType);
}

bool FactorNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // factor => ( expr )
    //         | Defparam
    //         | Num
    //         | matrix

    WordTypeEnum wordType = wordCursorRef.CurrentWord().WordType();
    if (MatrixNT::IsInFirstSet(wordType)) {
        // factor => matrix

        _pInnerNT = new MatrixNT();
        isSuccess = isSuccess && _pInnerNT->Parse(exprContextRef, wordCursorRef);
    }
    else if (FunctionNT::IsInFirstSet(wordType)) {
        // factor => Deffunc

        _pInnerNT = new FunctionNT();
        isSuccess = isSuccess && _pInnerNT->Parse(exprContextRef, wordCursorRef);
    }
    else {
        int leftParantheseIdx = 0;

        switch (wordType) {
            case WT_Paranthese_L :
                // factor => ( expr )

                leftParantheseIdx = wordCursorRef.CurrentIdx();
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                _pInnerNT = new ExprNT();
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
                break;
            case WT_DefVariable :
                // factor => Defparam

                _spExprILCode.reset(new PushDefValILCode(wordCursorRef.CurrentWord().StringValue(),
                    static_cast<ResultTypeEnum>(wordCursorRef.CurrentWord().IntValue())));
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                break;
            case WT_RealValue :
                // factor => Num

                _spExprILCode.reset(new PushRealValILCode(wordCursorRef.CurrentWord().RealValue()));
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
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
        return _spExprILCode->GetReturnType();
}

ExprILCodeSegment& FactorNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    if (nullptr != _pInnerNT) {
        _pInnerNT->AppendILSegment(ilSegment);
    }
    else {
        _ASSERT((bool)_spExprILCode);

        ilSegment.Append(_spExprILCode);
    }
    
    return ilSegment;
}
