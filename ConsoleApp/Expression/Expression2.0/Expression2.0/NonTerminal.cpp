#include "stdafx.h"
#include "NonTerminal.h"
#include "WordParser.h"
#include "ExprRunTime.h"
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
    if (NULL != _pExprILUnit) {
        delete _pExprILUnit;
        _pExprILUnit = NULL;
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

                _pExprILUnit = new ExprILUnit(EIL_PushRealVal, wordCursorRef.CurrentWord().RealValue());
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
ExprILSegment& StartNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    return _ListNT.AppendILSegment(exprILSegment);
}

//---------------------------------------------------------------------
// Class member - ListNT
//---------------------------------------------------------------------
ExprILSegment& ListNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    for (ExprList_t::iterator iter = _ExprList.begin(); _ExprList.end() != iter; ++iter) {
        (*iter)->AppendILSegment(exprILSegment);

        if (_IsEndWithSemicolon || _ExprList.back() != *iter) {
            // TODO: Remove run stack top node.
        }
    }

    return exprILSegment;
}

//---------------------------------------------------------------------
// Class member - ExprNT
//---------------------------------------------------------------------
ExprILSegment& ExprNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    _ASSERT(NULL != _pSubExprNT);

    return _pSubExprNT->AppendILSegment(exprILSegment);
}

//---------------------------------------------------------------------
// Class member - SubExprNT
//---------------------------------------------------------------------
ExprILSegment& SubExprNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    if (_IsFirstOne) {
        _TermNT.AppendILSegment(exprILSegment);
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        switch (_OperatorWordType) {
            case WT_Plus :
                AppendPlusIL(exprILSegment);
                break;
            case WT_Minus :
                AppendMinusIL(exprILSegment);
                break;
            default :
                _ASSERT(0);
                break;
        }
    }

    return exprILSegment;
}

void SubExprNT::AppendPlusIL(ExprILSegment &exprILSegment)
{
    _ASSERT(WT_Plus == _OperatorWordType);

    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _TermNT.ResultType();

    switch (resultType_L) {
        case RT_Matrix :
            if (RT_Matrix == resultType_R) {
                _pLeftOne->AppendILSegment(exprILSegment);
                _TermNT.AppendILSegment(exprILSegment);
                exprILSegment.Append(ExprILUnit(EIL_MatrixPlus));
            }
            else {
                _pLeftOne->AppendILSegment(exprILSegment);
                _TermNT.AppendILSegment(exprILSegment);
                exprILSegment.Append(ExprILUnit(EIL_MatrixValPlus));
            }
            break;
        case RT_RealVal :
            if (RT_Matrix == resultType_R) {
                _TermNT.AppendILSegment(exprILSegment);
                _pLeftOne->AppendILSegment(exprILSegment);
                exprILSegment.Append(ExprILUnit(EIL_MatrixValPlus));
            }
            else {
                _pLeftOne->AppendILSegment(exprILSegment);
                _TermNT.AppendILSegment(exprILSegment);
                exprILSegment.Append(ExprILUnit(EIL_RealValPlus));
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

void SubExprNT::AppendMinusIL(ExprILSegment &exprILSegment)
{
    _ASSERT(WT_Minus == _OperatorWordType);

    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _TermNT.ResultType();

    _ASSERT(RT_String != resultType_L && RT_String != resultType_R);

    if (RT_Matrix == resultType_L) {
        if (RT_Matrix == resultType_R) {
            _pLeftOne->AppendILSegment(exprILSegment);
            _TermNT.AppendILSegment(exprILSegment);
            exprILSegment.Append(ExprILUnit(EIL_MatrixMinus));
        }
        else {
            _pLeftOne->AppendILSegment(exprILSegment);
            _TermNT.AppendILSegment(exprILSegment);
            exprILSegment.Append(ExprILUnit(EIL_MatrixValMinus));
        }
    }
    else {
        if (RT_Matrix == resultType_R) {
            // Can not minus a 'real value' by a 'matrix'
            _ASSERT(0);
        }
        else {
            _pLeftOne->AppendILSegment(exprILSegment);
            _TermNT.AppendILSegment(exprILSegment);
            exprILSegment.Append(ExprILUnit(EIL_RealValMinus));
        }
    }
}

//---------------------------------------------------------------------
// Class member - TermNT
//---------------------------------------------------------------------
ExprILSegment& TermNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    _ASSERT(NULL != _pSubTermNT);

    return _pSubTermNT->AppendILSegment(exprILSegment);
}

//---------------------------------------------------------------------
// Class member - SubTermNT
//---------------------------------------------------------------------
ExprILSegment& SubTermNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    if (_IsFirstOne) {
        _FactorNT.AppendILSegment(exprILSegment);
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        switch (_OperatorWordType) {
            case WT_Multiply :
                AppendMultiplyIL(exprILSegment);
                break;
            case WT_DotMultiply:
                AppendDotMultiplyIL(exprILSegment);
                break;
            case WT_Divide :
                AppendDivideIL(exprILSegment);
                break;
            case WT_DotDivide :
                AppendDotDivideIL(exprILSegment);
                break;
            default :
                _ASSERT(0);
                break;
        }
    }

    return exprILSegment;
}

void SubTermNT::AppendMultiplyIL(ExprILSegment &exprILSegment)
{
    _ASSERT(WT_Multiply == _OperatorWordType);

    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _FactorNT.ResultType();

    if (RT_Matrix == resultType_L) {
        if (RT_Matrix == resultType_R) {
            _pLeftOne->AppendILSegment(exprILSegment);
            _FactorNT.AppendILSegment(exprILSegment);
            exprILSegment.Append(ExprILUnit(EIL_MatrixMultiply));
        }
        else {
            _pLeftOne->AppendILSegment(exprILSegment);
            _FactorNT.AppendILSegment(exprILSegment);
            exprILSegment.Append(ExprILUnit(EIL_MatrixValMultiply));
        }
    }
    else {
        if (RT_Matrix == resultType_R) {
            _FactorNT.AppendILSegment(exprILSegment);
            _pLeftOne->AppendILSegment(exprILSegment);
            exprILSegment.Append(ExprILUnit(EIL_MatrixValMultiply));
        }
        else {
            _pLeftOne->AppendILSegment(exprILSegment);
            _FactorNT.AppendILSegment(exprILSegment);
            exprILSegment.Append(ExprILUnit(EIL_RealValMultiply));
        }
    }
}

void SubTermNT::AppendDotMultiplyIL(ExprILSegment &exprILSegment)
{
    _ASSERT(WT_DotMultiply == _OperatorWordType);

    _pLeftOne->AppendILSegment(exprILSegment);
    _FactorNT.AppendILSegment(exprILSegment);

    _ASSERT(_pLeftOne->ResultType() == _FactorNT.ResultType());

    if (RT_Matrix == _pLeftOne->ResultType())
        exprILSegment.Append(ExprILUnit(EIL_MatrixDotMultiply));
    if (RT_RealVal == _pLeftOne->ResultType())
        exprILSegment.Append(ExprILUnit(EIL_RealValMultiply));
}

void SubTermNT::AppendDivideIL(ExprILSegment &exprILSegment)
{
    _ASSERT(WT_Divide == _OperatorWordType);

    ResultTypeEnum resultType_L = _pLeftOne->ResultType();
    ResultTypeEnum resultType_R = _FactorNT.ResultType();

    _pLeftOne->AppendILSegment(exprILSegment);
    _FactorNT.AppendILSegment(exprILSegment);

    if (RT_Matrix == resultType_L) {
        if (RT_Matrix == resultType_L)
            exprILSegment.Append(ExprILUnit(EIL_MatrixDivide));
        else
            exprILSegment.Append(ExprILUnit(EIL_MatrixValDivide));
    }
    else {
        if (RT_Matrix == resultType_L)
            _ASSERT(0);
        else
            exprILSegment.Append(ExprILUnit(EIL_RealValDivide));
    }
}

void SubTermNT::AppendDotDivideIL(ExprILSegment &exprILSegment)
{
    _ASSERT(WT_DotDivide == _OperatorWordType);

    _pLeftOne->AppendILSegment(exprILSegment);
    _FactorNT.AppendILSegment(exprILSegment);

    _ASSERT(_pLeftOne->ResultType() == _FactorNT.ResultType());

    if (RT_Matrix == _pLeftOne->ResultType())
        exprILSegment.Append(ExprILUnit(EIL_MatrixDotDivide));
    if (RT_RealVal == _pLeftOne->ResultType())
        exprILSegment.Append(ExprILUnit(EIL_RealValDivide));
}

//---------------------------------------------------------------------
// Class member - FactorNT
//---------------------------------------------------------------------
ExprILSegment& FactorNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    if (NULL != _pExprNT)
        _pExprNT->AppendILSegment(exprILSegment);
    else if (NULL != _pMatrixNT)
        _pMatrixNT->AppendILSegment(exprILSegment);
    else
        exprILSegment.Append(*_pExprILUnit);

    return exprILSegment;
}

//---------------------------------------------------------------------
// Class member - MatrixNT
//---------------------------------------------------------------------
ExprILSegment& MatrixNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    _MatrixRows.AppendILSegment(exprILSegment);
    exprILSegment.Append(ExprILUnit(EIL_CtorMatrix, Rows(), Cols()));

    return exprILSegment;
}

//---------------------------------------------------------------------
// Class member - MatrixRowsNT
//---------------------------------------------------------------------
ExprILSegment& MatrixRowsNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    for (RowList_t::iterator iter = _RowList.begin(); _RowList.end() != iter; ++iter)
        (*iter)->AppendILSegment(exprILSegment);

    return exprILSegment;
}

//---------------------------------------------------------------------
// Class member - MatrixColsNT
//---------------------------------------------------------------------
ExprILSegment& MatrixColsNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    for (ColList_t::iterator iter = _ColList.begin(); _ColList.end() != iter; ++iter)
        (*iter)->AppendILSegment(exprILSegment);

    return exprILSegment;
}

//---------------------------------------------------------------------
// Class member - MatrixColNT
//---------------------------------------------------------------------
ExprILSegment& MatrixColNT::AppendILSegment(ExprILSegment &exprILSegment)
{
    return _ExprNT.AppendILSegment(exprILSegment);
}