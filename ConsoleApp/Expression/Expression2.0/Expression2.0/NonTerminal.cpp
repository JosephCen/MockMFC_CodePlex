#include "stdafx.h"
#include "NonTerminal.h"
#include "WordParser.h"
#include "ExprILCode.h"
#include "ExprWorkSpace.h"
#include <list>
using std::list;
#include <crtdbg.h>

//---------------------------------------------------------------------
// Class member - BaseNonTerminal
//---------------------------------------------------------------------
ResultTypeEnum BaseNonTerminal::ResultType(void)
{
    if (RT_None == _ResultType)
        _ResultType = GetResultType();

    return _ResultType;
}

//---------------------------------------------------------------------
// Class member - StartNT
//---------------------------------------------------------------------
bool StartNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(start) = { ( | Defparam | Num | [ | nul | Eof }
    return WT_Paranthese_L == wordType || WT_DefVariable == wordType || WT_RealValue == wordType
           || WT_S_Bracket_L == wordType || WT_Eof == wordType;
}

bool StartNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // start => list Eof

    isSuccess = isSuccess && _ListNT.Parse(exprContextRef, wordCursorRef);
    if (isSuccess)
    {
        if (WT_Eof != wordCursorRef.CurrentWord().WordType())
        {
            exprContextRef.SetError("A unexpected word.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }

    return isSuccess;
}

//---------------------------------------------------------------------
// Class member - ListNT
//---------------------------------------------------------------------
ListNT::~ListNT()
{
    for (ExprList_t::iterator iter = _ExprList.begin(); _ExprList.end() != iter; ++iter) {
        delete *iter;
        *iter = NULL;
    }
}

bool ListNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(list) = { ( | Defparam | Num | [ | nul }
    return WT_Paranthese_L == wordType || WT_DefVariable == wordType || WT_RealValue == wordType
           || WT_S_Bracket_L == wordType || WT_Nul == wordType;
}

bool ListNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // list => expr ; list
    //       | nul

    while (isSuccess && ExprNT::IsInFirstSet(wordCursorRef.CurrentWord().WordType())) {
        _ExprList.push_back(new ExprNT());
        isSuccess = isSuccess && _ExprList.back()->Parse(exprContextRef, wordCursorRef);
        if (isSuccess) {
            if (WT_Semicolon == wordCursorRef.CurrentWord().WordType()) {
                _IsEndWithSemicolon = true;
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            }
            else {
                _IsEndWithSemicolon = false;
                break;
            }
        }
    }

    return isSuccess;
}

//---------------------------------------------------------------------
// Class member - ExprNT
//---------------------------------------------------------------------
ExprNT::~ExprNT()
{
    if (NULL != _pSubExprNT) {
        delete _pSubExprNT;
        _pSubExprNT = NULL;
    }
}

bool ExprNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(expr) = { ( | Defparam | Num | [ }
    return WT_Paranthese_L == wordType || WT_DefVariable == wordType || WT_RealValue == wordType || WT_S_Bracket_L == wordType;
}

bool ExprNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;
    //ResultTypeEnum resultType_L;
    //ResultTypeEnum resultType_R;

    // expr => term moreterms
    // moreterms => + term moreterms
    //            | - term moreterms
    //            | nul

    _pSubExprNT = new SubExprNT();
    isSuccess = isSuccess && _pSubExprNT->Parse(exprContextRef, wordCursorRef);

    while (isSuccess && SubExprNT::IsInFirstSet(wordCursorRef.CurrentWord().WordType(), false)) {
        _pSubExprNT = new SubExprNT(_pSubExprNT);
        isSuccess = isSuccess && _pSubExprNT->Parse(exprContextRef, wordCursorRef);
    }

    return isSuccess;
}

ResultTypeEnum ExprNT::GetResultType(void)
{
    _ASSERT(NULL != _pSubExprNT);
    
    return _pSubExprNT->ResultType();
}

//---------------------------------------------------------------------
// Class member - SubExprNT
//---------------------------------------------------------------------
SubExprNT::SubExprNT(void):
_IsFirstOne(true), _pLeftOne(NULL), _OperatorWordType(WT_Nul), _TermNT(), BaseNonTerminal()
{ }

SubExprNT::SubExprNT(SubExprNT *pLeftOne):
_IsFirstOne(false), _pLeftOne(pLeftOne), _OperatorWordType(WT_Nul), _TermNT(), BaseNonTerminal()
{
    _ASSERT(NULL != pLeftOne);
}

SubExprNT::~SubExprNT()
{
    if (NULL != _pLeftOne) {
        delete _pLeftOne;
        _pLeftOne = NULL;
    }
}

bool SubExprNT::IsInFirstSet(WordTypeEnum wordType, bool isFirstOne)
{
    // expr => term moreterms
    // moreterms => + term moreterms
    //            | - term moreterms
    //            | nul

    if (isFirstOne)
        return TermNT::IsInFirstSet(wordType);
    else
        return WT_Plus == wordType || WT_Minus == wordType;
}

bool SubExprNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // expr => term moreterms
    // moreterms => + term moreterms
    //            | - term moreterms
    //            | nul

    if (_IsFirstOne) {
        // => term

        isSuccess = isSuccess && _TermNT.Parse(exprContextRef, wordCursorRef);
    }
    else {
        // => + term moreterms
        //  | - term moreterms
        //  | nul

        if (IsInFirstSet(wordCursorRef.CurrentWord().WordType(), false)) {
            int operatorWordIdx = wordCursorRef.CurrentIdx();
            _OperatorWordType = wordCursorRef.CurrentWord().WordType();
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            isSuccess = isSuccess && _TermNT.Parse(exprContextRef, wordCursorRef);
            isSuccess = isSuccess && OperatorValidate(exprContextRef, operatorWordIdx);
        }
        else {
            exprContextRef.SetError("Expect a plus or a minus operator.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }

    return isSuccess;
}

bool SubExprNT::OperatorValidate(ExprContext &exprContextRef, int operatorWordIdx)
{
    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _TermNT.ResultType();

    if (resultType_L == resultType_R) {
        if (WT_Minus == _OperatorWordType && RT_String == resultType_R) {
            exprContextRef.SetError("Minus can not be done between two 'string' operand.", operatorWordIdx);

            return false;
        }
    }
    else {
        if (RT_String == resultType_L || RT_String == resultType_R) {
            exprContextRef.SetError("Plus or minus can not be done between a 'string' operand and a 'real value' or a 'matrix' operand.", operatorWordIdx);

            return false;
        }
        if (WT_Minus == _OperatorWordType && (RT_RealVal == resultType_L || RT_Matrix == resultType_R)) {
            exprContextRef.SetError("Can not minus a 'real value' by a 'matrix'.", operatorWordIdx);

            return false;
        }
    }

    return true;
}

ResultTypeEnum SubExprNT::GetResultType(void)
{
    if (_IsFirstOne) {
        return _TermNT.ResultType();
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        ResultTypeEnum resultType_L = _pLeftOne->ResultType();
        ResultTypeEnum resultType_R = _TermNT.ResultType();

        if (resultType_L == resultType_R) {
            return resultType_R;
        }
        else {
            if (RT_Matrix == _pLeftOne->ResultType() || RT_Matrix == _TermNT.ResultType())
            return RT_Matrix;
        else
            return RT_RealVal;
        }
    }
}

//---------------------------------------------------------------------
// Class member - TermNT
//---------------------------------------------------------------------
TermNT::~TermNT()
{
    if (NULL != _pSubTermNT) {
        delete _pSubTermNT;
        _pSubTermNT = NULL;
    }
}

bool TermNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(term) = { ( | Defparam | Num | [ }
    return WT_Paranthese_L == wordType || WT_DefVariable == wordType || WT_RealValue == wordType || WT_S_Bracket_L == wordType;
}

bool TermNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;
    //ResultTypeEnum resultType_L;
    //ResultTypeEnum resultType_R;

    // term => factor morefactors
    // morefactors => * factor morefactors
    //              | / factor morefactors
    //              | .* factor morefactors
    //              | ./ factor morefactors
    //              | nul

    _pSubTermNT = new SubTermNT();
    isSuccess = isSuccess && _pSubTermNT->Parse(exprContextRef, wordCursorRef);

    while (isSuccess && SubTermNT::IsInFirstSet(wordCursorRef.CurrentWord().WordType(), false)) {
        _pSubTermNT = new SubTermNT(_pSubTermNT);
        isSuccess = isSuccess && _pSubTermNT->Parse(exprContextRef, wordCursorRef);
    }

    return isSuccess;
}

ResultTypeEnum TermNT::GetResultType(void)
{
    _ASSERT(NULL != _pSubTermNT);
    return _pSubTermNT->ResultType();
}

//---------------------------------------------------------------------
// Class member - SubTermNT
//---------------------------------------------------------------------
SubTermNT::SubTermNT(void):
_IsFirstOne(true), _pLeftOne(NULL), _OperatorWordType(WT_Nul), _FactorNT(), BaseNonTerminal()
{ }

SubTermNT::SubTermNT(SubTermNT *pLeftOne):
_IsFirstOne(false), _pLeftOne(pLeftOne), _OperatorWordType(WT_Nul), _FactorNT(), BaseNonTerminal()
{
    _ASSERT(NULL != pLeftOne);
}

SubTermNT::~SubTermNT()
{
    if (NULL != _pLeftOne) {
        delete _pLeftOne;
        _pLeftOne = NULL;
    }
}

bool SubTermNT::IsInFirstSet(WordTypeEnum wordType, bool isFirstOne)
{
    // term => factor morefactors
    // morefactors => * factor morefactors
    //              | / factor morefactors
    //              | .* factor morefactors
    //              | ./ factor morefactors
    //              | nul

    if (isFirstOne)
        return FactorNT::IsInFirstSet(wordType);
    else
        return WT_Multiply == wordType || WT_Divide == wordType || WT_DotMultiply == wordType || WT_DotDivide == wordType;
}

bool SubTermNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // term => factor morefactors
    // morefactors => * factor morefactors
    //              | / factor morefactors
    //              | .* factor morefactors
    //              | ./ factor morefactors
    //              | nul

    if (_IsFirstOne) {
        // => factors

        isSuccess = isSuccess && _FactorNT.Parse(exprContextRef, wordCursorRef);
    }
    else {
        // => * factor morefactors
        //  | / factor morefactors
        //  | .* factor morefactors
        //  | ./ factor morefactors

        if (IsInFirstSet(wordCursorRef.CurrentWord().WordType(), false)) {
            int operatorWordIdx = wordCursorRef.CurrentIdx();
            _OperatorWordType = wordCursorRef.CurrentWord().WordType();
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            isSuccess = isSuccess && _FactorNT.Parse(exprContextRef, wordCursorRef);
            isSuccess = isSuccess && OperatorValidate(exprContextRef, operatorWordIdx);
        }
        else {
            exprContextRef.SetError("Expect a divide or a multiply operator.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }

    return isSuccess;
}

bool SubTermNT::OperatorValidate(ExprContext &exprContextRef, int operatorWordIdx)
{
    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _FactorNT.ResultType();

    if (RT_String == resultType_L || RT_String == resultType_R) {
        exprContextRef.SetError("Divide or multiply operator can not take a 'string' operand.", operatorWordIdx);
        
        return false;
    }
    if (WT_DotMultiply == _OperatorWordType) {
        if ((RT_Matrix == resultType_L && RT_RealVal == resultType_R) || RT_RealVal == resultType_L && RT_Matrix == resultType_R) {
            exprContextRef.SetError("Dot multiply operator can not be done between a 'real value' operand and a 'matrix' operand.", operatorWordIdx);
        
            return false;
        }
    }
    if (WT_DotDivide == _OperatorWordType) {
        if ((RT_Matrix == resultType_L && RT_RealVal == resultType_R) || RT_RealVal == resultType_L && RT_Matrix == resultType_R) {
            exprContextRef.SetError("Dot divide operator can not be done between a 'real value' operand and a 'matrix' operand.", operatorWordIdx);
        
            return false;
        }
    }
    if (WT_Divide == _OperatorWordType) {
        if (RT_RealVal == resultType_L && RT_Matrix == resultType_R) {
            exprContextRef.SetError("Can not divide a 'real value' by a 'matrix' divisor.", operatorWordIdx);

            return false;
        }
    }

    return true;
}

ResultTypeEnum SubTermNT::GetResultType(void)
{
    if (_IsFirstOne) {
        return _FactorNT.ResultType();
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        if (RT_Matrix == _pLeftOne->ResultType() || RT_Matrix == _FactorNT.ResultType())
            return RT_Matrix;
        else
            return RT_RealVal;
    }
}

//---------------------------------------------------------------------
// Class member - FactorNT
//---------------------------------------------------------------------
FactorNT::~FactorNT()
{
    if (NULL != _pExprNT) {
        delete _pExprNT;
        _pExprNT = NULL;
    }
    if (NULL != _pMatrixNT) {
        delete _pMatrixNT;
        _pMatrixNT = NULL;
    }
    if (NULL != _pExprILCode) {
        delete _pExprILCode;
        _pExprILCode = NULL;
    }
}

bool FactorNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(factor) = { ( | Defparam | Num | [ }
    return WT_Paranthese_L == wordType || WT_DefVariable == wordType || WT_RealValue == wordType || WT_S_Bracket_L == wordType;
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

        _pMatrixNT = new MatrixNT();
        isSuccess = isSuccess && _pMatrixNT->Parse(exprContextRef, wordCursorRef);
    }
    else {
        int leftParantheseIdx = 0;

        switch (wordType) {
            case WT_Paranthese_L :
                // factor => ( expr )

                leftParantheseIdx = wordCursorRef.CurrentIdx();
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                _pExprNT = new ExprNT();
                isSuccess = isSuccess && _pExprNT->Parse(exprContextRef, wordCursorRef);
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

                // TODO: Parse a define variable
                _ASSERT(0);
                break;
            case WT_RealValue :
                // factor => Num

                _pExprILCode = new PushRealValILCode(wordCursorRef.CurrentWord().RealValue());
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
                if (isSuccess)
                    _ResultType = RT_RealVal;
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
    if (NULL != _pExprNT)
        return _pExprNT->ResultType();
    else if (NULL != _pMatrixNT)
        return _pMatrixNT->ResultType();
    else
        return _ResultType;
}

//---------------------------------------------------------------------
// Class member - MatrixNT
//---------------------------------------------------------------------
bool MatrixNT::IsInFirstSet(WordTypeEnum wordType)
{
    // First(matrix) = { [ }
    return WT_S_Bracket_L == wordType;
}

bool MatrixNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;
    int leftSquareBracketIdx = 0;

    // matrix => [ rows ]
    if (WT_S_Bracket_L == wordCursorRef.CurrentWord().WordType()) {
        leftSquareBracketIdx = wordCursorRef.CurrentIdx();
        isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
    }
    else {
        isSuccess = false;
        exprContextRef.SetError("Unexpected word. Expect a left sqare bracket.", wordCursorRef.CurrentIdx());
    }
    isSuccess = isSuccess && _MatrixRows.Parse(exprContextRef, wordCursorRef);
    if (isSuccess) {
        if (WT_S_Bracket_R == wordCursorRef.CurrentWord().WordType()) {
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
        }
        else {
            exprContextRef.SetError("Square bracket do not match. Missing the right square bracket.", leftSquareBracketIdx);
            isSuccess = false;
        }
    }

    return isSuccess;
}

//---------------------------------------------------------------------
// Class member - MatrixRowsNT
//---------------------------------------------------------------------
MatrixRowsNT::~MatrixRowsNT()
{
    for (RowList_t::iterator iter = _RowList.begin(); _RowList.end() != iter; ++iter) {
        delete *iter;
        *iter = NULL;
    }
}

bool MatrixRowsNT::IsInFirstSet(WordTypeEnum wordType)
{
    // rows => cols
    //       | cols ; rows
    return MatrixColsNT::IsInFirstSet(wordType);
}

bool MatrixRowsNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    _ASSERT(IsInFirstSet(wordCursorRef.CurrentWord().WordType()));

    bool isSuccess = true;
    Matrix::Row_Col_t cols = 0;

    // rows => cols
    //       | cols ; rows
    do {
        _RowList.push_back(new MatrixColsNT());
        if (_RowList.back()->Parse(exprContextRef, wordCursorRef)) {
            cols = (0 == cols ? _RowList.back()->Cols() : cols);
            if (cols != _RowList.back()->Cols()) {
                exprContextRef.SetError("Cols in a matrix could not be different.", wordCursorRef.CurrentIdx());
                isSuccess = false;
            }
        }
        else {
            isSuccess = false;
        }
        if (isSuccess && WT_Semicolon == wordCursorRef.CurrentWord().WordType())
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
        else
            break;
    } while (isSuccess);

    return isSuccess;
}

//---------------------------------------------------------------------
// Class member - MatrixColsNT
//---------------------------------------------------------------------
MatrixColsNT::~MatrixColsNT()
{
    for (ColList_t::iterator iter = _ColList.begin(); _ColList.end() != iter; ++iter) {
        delete *iter;
        *iter = NULL;
    }
}

bool MatrixColsNT::IsInFirstSet(WordTypeEnum wordType)
{
    // cols => col
    //       | col , cols
    return MatrixColNT::IsInFirstSet(wordType);
}

bool MatrixColsNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    _ASSERT(IsInFirstSet(wordCursorRef.CurrentWord().WordType()));

    bool isSuccess = true;

    // cols => col
    //       | col , cols

    do {
        _ColList.push_back(new MatrixColNT());
        isSuccess = isSuccess && _ColList.back()->Parse(exprContextRef, wordCursorRef);
        if (isSuccess && WT_Comma == wordCursorRef.CurrentWord().WordType())
            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
        else
            break;
    } while (isSuccess);

    return isSuccess;
}

//---------------------------------------------------------------------
// Class member - MatrixColNT
//---------------------------------------------------------------------
bool MatrixColNT::IsInFirstSet(WordTypeEnum wordType)
{
    // col => expr
    return ExprNT::IsInFirstSet(wordType);
}

bool MatrixColNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    bool isSuccess = true;

    // col => expr

    if (IsInFirstSet(wordCursorRef.CurrentWord().WordType())) {
        int exprStartIdx = wordCursorRef.CurrentIdx();
        isSuccess = isSuccess && _ExprNT.Parse(exprContextRef, wordCursorRef);
        if (isSuccess && RT_String == _ExprNT.ResultType()) {
            isSuccess = false;
            exprContextRef.SetError("Just real value or matrix could be contain in a matrix", exprStartIdx);
        }
    }
    else {
        isSuccess = false;
        exprContextRef.SetError("Unexpected word. Expect a real value or an sub expression.", wordCursorRef.CurrentIdx());
    }

    return isSuccess;
}

//---------------------------------------------------------------------
// IL generate code
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// Class member - StartNT
//---------------------------------------------------------------------
ExprILCodeSegment& StartNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    return _ListNT.AppendILSegment(ilSegment);
}

//---------------------------------------------------------------------
// Class member - ListNT
//---------------------------------------------------------------------
ExprILCodeSegment& ListNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    for (ExprList_t::iterator iter = _ExprList.begin(); _ExprList.end() != iter; ++iter) {
        (*iter)->AppendILSegment(ilSegment);

        if (_IsEndWithSemicolon || _ExprList.back() != *iter) {
            // TODO: Remove run stack top node.
        }
    }

    return ilSegment;
}

//---------------------------------------------------------------------
// Class member - ExprNT
//---------------------------------------------------------------------
ExprILCodeSegment& ExprNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    _ASSERT(NULL != _pSubExprNT);

    return _pSubExprNT->AppendILSegment(ilSegment);
}

//---------------------------------------------------------------------
// Class member - SubExprNT
//---------------------------------------------------------------------
ExprILCodeSegment& SubExprNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    if (_IsFirstOne) {
        _TermNT.AppendILSegment(ilSegment);
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        switch (_OperatorWordType) {
            case WT_Plus :
                AppendPlusIL(ilSegment);
                break;
            case WT_Minus :
                AppendMinusIL(ilSegment);
                break;
            default :
                _ASSERT(0);
                break;
        }
    }

    return ilSegment;
}

void SubExprNT::AppendPlusIL(ExprILCodeSegment &ilSegment)
{
    _ASSERT(WT_Plus == _OperatorWordType);

    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _TermNT.ResultType();

    switch (resultType_L) {
        case RT_Matrix :
            if (RT_Matrix == resultType_R) {
                _pLeftOne->AppendILSegment(ilSegment);
                _TermNT.AppendILSegment(ilSegment);
                ilSegment.Append(new MatrixPlusILCode());
            }
            else {
                _pLeftOne->AppendILSegment(ilSegment);
                _TermNT.AppendILSegment(ilSegment);
                ilSegment.Append(new MatrixValPlusILCode());
            }
            break;
        case RT_RealVal :
            if (RT_Matrix == resultType_R) {
                _TermNT.AppendILSegment(ilSegment);
                _pLeftOne->AppendILSegment(ilSegment);
                ilSegment.Append(new MatrixValPlusILCode());
            }
            else {
                _pLeftOne->AppendILSegment(ilSegment);
                _TermNT.AppendILSegment(ilSegment);
                ilSegment.Append(new RealValPlusILCode());
            }
            break;
        case RT_String :
            _ASSERT(resultType_L == resultType_R);
            // TODO: String add operation.
            break;
        default :
            _ASSERT(0);
            break;
    }
}

void SubExprNT::AppendMinusIL(ExprILCodeSegment &ilSegment)
{
    _ASSERT(WT_Minus == _OperatorWordType);

    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _TermNT.ResultType();

    _ASSERT(RT_String != resultType_L && RT_String != resultType_R);

    if (RT_Matrix == resultType_L) {
        if (RT_Matrix == resultType_R) {
            _pLeftOne->AppendILSegment(ilSegment);
            _TermNT.AppendILSegment(ilSegment);
            ilSegment.Append(new MatrixMinusILCode());
        }
        else {
            _pLeftOne->AppendILSegment(ilSegment);
            _TermNT.AppendILSegment(ilSegment);
            ilSegment.Append(new MatrixValMinusILCode());
        }
    }
    else {
        if (RT_Matrix == resultType_R) {
            // Can not minus a 'real value' by a 'matrix'
            _ASSERT(0);
        }
        else {
            _pLeftOne->AppendILSegment(ilSegment);
            _TermNT.AppendILSegment(ilSegment);
            ilSegment.Append(new RealValMinusILCode());
        }
    }
}

//---------------------------------------------------------------------
// Class member - TermNT
//---------------------------------------------------------------------
ExprILCodeSegment& TermNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    _ASSERT(NULL != _pSubTermNT);

    return _pSubTermNT->AppendILSegment(ilSegment);
}

//---------------------------------------------------------------------
// Class member - SubTermNT
//---------------------------------------------------------------------
ExprILCodeSegment& SubTermNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    if (_IsFirstOne) {
        _FactorNT.AppendILSegment(ilSegment);
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        switch (_OperatorWordType) {
            case WT_Multiply :
                AppendMultiplyIL(ilSegment);
                break;
            case WT_DotMultiply:
                AppendDotMultiplyIL(ilSegment);
                break;
            case WT_Divide :
                AppendDivideIL(ilSegment);
                break;
            case WT_DotDivide :
                AppendDotDivideIL(ilSegment);
                break;
            default :
                _ASSERT(0);
                break;
        }
    }

    return ilSegment;
}

void SubTermNT::AppendMultiplyIL(ExprILCodeSegment &ilSegment)
{
    _ASSERT(WT_Multiply == _OperatorWordType);

    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _FactorNT.ResultType();

    if (RT_Matrix == resultType_L) {
        if (RT_Matrix == resultType_R) {
            _pLeftOne->AppendILSegment(ilSegment);
            _FactorNT.AppendILSegment(ilSegment);
            // TODO:
            _ASSERT(0);
            // ilSegment.Append(new MatrixMultiplyILCode());
        }
        else {
            _pLeftOne->AppendILSegment(ilSegment);
            _FactorNT.AppendILSegment(ilSegment);
            ilSegment.Append(new MatrixValMultiplyILCode());
        }
    }
    else {
        if (RT_Matrix == resultType_R) {
            _FactorNT.AppendILSegment(ilSegment);
            _pLeftOne->AppendILSegment(ilSegment);
            ilSegment.Append(new MatrixValMultiplyILCode());
        }
        else {
            _pLeftOne->AppendILSegment(ilSegment);
            _FactorNT.AppendILSegment(ilSegment);
            ilSegment.Append(new RealValMultiplyILCode());
        }
    }
}

void SubTermNT::AppendDotMultiplyIL(ExprILCodeSegment &ilSegment)
{
    _ASSERT(WT_DotMultiply == _OperatorWordType);

    _pLeftOne->AppendILSegment(ilSegment);
    _FactorNT.AppendILSegment(ilSegment);

    _ASSERT(_pLeftOne->ResultType() == _FactorNT.ResultType());

    if (RT_Matrix == _pLeftOne->ResultType())
        ilSegment.Append(new MatrixDotMultiplyILCode());
    if (RT_RealVal == _pLeftOne->ResultType())
        ilSegment.Append(new RealValMultiplyILCode());
}

void SubTermNT::AppendDivideIL(ExprILCodeSegment &ilSegment)
{
    _ASSERT(WT_Divide == _OperatorWordType);

    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _FactorNT.ResultType();

    _pLeftOne->AppendILSegment(ilSegment);
    _FactorNT.AppendILSegment(ilSegment);

    if (RT_Matrix == resultType_L) {
        if (RT_Matrix == resultType_L)
            // TODO:
            _ASSERT(0);
            //ilSegment.Append(new MatrixDivideILCode());
        else
            ilSegment.Append(new MatrixValDivideILCode());
    }
    else {
        if (RT_Matrix == resultType_L)
            // TODO:
            _ASSERT(0);
        else
            ilSegment.Append(new RealValDivideILCode());
    }
}

void SubTermNT::AppendDotDivideIL(ExprILCodeSegment &ilSegment)
{
    _ASSERT(WT_DotDivide == _OperatorWordType);

    _pLeftOne->AppendILSegment(ilSegment);
    _FactorNT.AppendILSegment(ilSegment);

    _ASSERT(_pLeftOne->ResultType() == _FactorNT.ResultType());

    if (RT_Matrix == _pLeftOne->ResultType())
        ilSegment.Append(new MatrixDotDivideILCode());
    if (RT_RealVal == _pLeftOne->ResultType())
        ilSegment.Append(new RealValDivideILCode());
}

//---------------------------------------------------------------------
// Class member - FactorNT
//---------------------------------------------------------------------
ExprILCodeSegment& FactorNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    if (NULL != _pExprNT) {
        _pExprNT->AppendILSegment(ilSegment);
    }
    else if (NULL != _pMatrixNT) {
        _pMatrixNT->AppendILSegment(ilSegment);
    }
    else {
        ilSegment.Append(_pExprILCode);
        _pExprILCode = NULL;
    }

    return ilSegment;
}

//---------------------------------------------------------------------
// Class member - MatrixNT
//---------------------------------------------------------------------
ExprILCodeSegment& MatrixNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    _MatrixRows.AppendILSegment(ilSegment);
    ilSegment.Append(new CtorMatrixILCode(Rows(), Cols()));

    return ilSegment;
}

//---------------------------------------------------------------------
// Class member - MatrixRowsNT
//---------------------------------------------------------------------
ExprILCodeSegment& MatrixRowsNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    for (RowList_t::iterator iter = _RowList.begin(); _RowList.end() != iter; ++iter)
        (*iter)->AppendILSegment(ilSegment);

    return ilSegment;
}

//---------------------------------------------------------------------
// Class member - MatrixColsNT
//---------------------------------------------------------------------
ExprILCodeSegment& MatrixColsNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    for (ColList_t::iterator iter = _ColList.begin(); _ColList.end() != iter; ++iter)
        (*iter)->AppendILSegment(ilSegment);

    return ilSegment;
}

//---------------------------------------------------------------------
// Class member - MatrixColNT
//---------------------------------------------------------------------
ExprILCodeSegment& MatrixColNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    return _ExprNT.AppendILSegment(ilSegment);
}