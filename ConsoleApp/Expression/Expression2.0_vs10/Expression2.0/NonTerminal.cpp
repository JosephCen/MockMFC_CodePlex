#include "stdafx.h"
#include "NonTerminal.h"
#include "WordParser.h"
#include "ExprILCode.h"
#include "ExprWorkSpace.h"
#include "ExprILHelper.h"
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - BaseNonTerminal
//---------------------------------------------------------------------
ResultTypeEnum BaseNonTerminal::ResultType(void)
{
    if (RT_None == _ResultType)
        _ResultType = GetResultType();

    return _ResultType;
}

ExprILCode* BaseNonTerminal::FindExprILCode(const FuncParamsInfo &funcInfo)
{
    // TODO: Add logic of override operator

    return ExprILHelper::FindOperatorILCode(funcInfo);
}

ExprILCode* BaseNonTerminal::FindExprILCode(WordTypeEnum operWordType, ResultTypeEnum lParamType, ResultTypeEnum rParamType)
{
    return ExprILHelper::FindOperatorILCode(operWordType, lParamType, rParamType);
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
    return TermNT::IsInFirstSet(wordType);
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
_IsFirstOne(true), _pLeftOne(NULL), _pExprILCode(NULL), _TermNT(), BaseNonTerminal()
{ }

SubExprNT::SubExprNT(SubExprNT *pLeftOne):
_IsFirstOne(false), _pLeftOne(pLeftOne), _pExprILCode(NULL), _TermNT(), BaseNonTerminal()
{
    _ASSERT(NULL != pLeftOne);
}

SubExprNT::~SubExprNT()
{
    delete _pLeftOne;
    _pLeftOne = NULL;
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
            WordTypeEnum operatorWordType = wordCursorRef.CurrentWord().WordType();

            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            isSuccess = isSuccess && _TermNT.Parse(exprContextRef, wordCursorRef);
            isSuccess = isSuccess && OperatorValidate(exprContextRef, operatorWordType, operatorWordIdx);
        }
        else {
            exprContextRef.SetError("Expect a plus or a minus operator.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }

    return isSuccess;
}

bool SubExprNT::OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx)
{
    _pExprILCode = FindExprILCode(operWordType, _pLeftOne->ResultType(), _TermNT.ResultType());

    if (NULL != _pExprILCode) {
        return true;
    }
    else {
        string errStr = ExprILHelper::GenOperNotFoundErr(operWordType, _pLeftOne->ResultType(), _TermNT.ResultType());

        exprContextRef.SetError(errStr.c_str(), operWordIdx);

        return false;
    }
}

ResultTypeEnum SubExprNT::GetResultType(void)
{
    if (_IsFirstOne) {
        return _TermNT.ResultType();
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        return (_pExprILCode->GetReturnType());
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
    return FactorNT::IsInFirstSet(wordType);
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
_IsFirstOne(true), _pLeftOne(NULL), _pExprILCode(NULL), _FactorNT(), BaseNonTerminal()
{ }

SubTermNT::SubTermNT(SubTermNT *pLeftOne):
_IsFirstOne(false), _pLeftOne(pLeftOne), _pExprILCode(NULL), _FactorNT(), BaseNonTerminal()
{
    _ASSERT(NULL != pLeftOne);
}

SubTermNT::~SubTermNT()
{
    delete _pLeftOne;
    _pLeftOne = NULL;
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
            WordTypeEnum operatorWordType = wordCursorRef.CurrentWord().WordType();

            isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            isSuccess = isSuccess && _FactorNT.Parse(exprContextRef, wordCursorRef);
            isSuccess = isSuccess && OperatorValidate(exprContextRef, operatorWordType, operatorWordIdx);
        }
        else {
            exprContextRef.SetError("Expect a divide or a multiply operator.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }

    return isSuccess;
}

bool SubTermNT::OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx)
{
    _pExprILCode = FindExprILCode(operWordType, _pLeftOne->ResultType(), _FactorNT.ResultType());

    if (NULL != _pExprILCode) {
        return true;
    }
    else {
        string errStr = ExprILHelper::GenOperNotFoundErr(operWordType, _pLeftOne->ResultType(), _FactorNT.ResultType());

        exprContextRef.SetError(errStr.c_str(), operWordIdx);

        return false;
    }
}

ResultTypeEnum SubTermNT::GetResultType(void)
{
    if (_IsFirstOne) {
        return _FactorNT.ResultType();
    }
    else {
        _ASSERT(NULL != _pLeftOne);

        return (_pExprILCode->GetReturnType());
    }
}

//---------------------------------------------------------------------
// Class member - FactorNT
//---------------------------------------------------------------------
FactorNT::FactorNT(void): 
_pExprNT(NULL), _pMatrixNT(NULL), _pExprILCode(NULL), _pFunctionNT(NULL), BaseNonTerminal()
{ }

FactorNT::~FactorNT()
{
    delete _pExprNT;
    _pExprNT = NULL;

    delete _pMatrixNT;
    _pMatrixNT = NULL;

    delete _pFunctionNT;
    _pFunctionNT = NULL;

    delete _pExprILCode;
    _pExprILCode = NULL;
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

        _pMatrixNT = new MatrixNT();
        isSuccess = isSuccess && _pMatrixNT->Parse(exprContextRef, wordCursorRef);
    }
    else if (FunctionNT::IsInFirstSet(wordType)) {
        // factor => Deffunc

        _pFunctionNT = new FunctionNT();
        isSuccess = isSuccess && _pFunctionNT->Parse(exprContextRef, wordCursorRef);
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
    else if (NULL != _pFunctionNT)
        return _pFunctionNT->ResultType();
    else
        return _pExprILCode->GetReturnType();
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
// Class member - FunctionNT
//---------------------------------------------------------------------
bool FunctionNT::IsInFirstSet(WordTypeEnum wordType)
{
    // function => deffunc (expr, ...)
    return WT_DefFunction == wordType;
}

FunctionNT::FunctionNT(void):
_ExprVec(), _pExprILCode(NULL), BaseNonTerminal()
{ }

bool FunctionNT::Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef)
{
    _ASSERT(IsInFirstSet(wordCursorRef.CurrentWord().WordType()));

    bool isSuccess = true;
    int operatorWordIdx = wordCursorRef.CurrentIdx();
    string funcName = wordCursorRef.CurrentWord().StringValue();

    isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
    if (isSuccess) {
        if (WT_Paranthese_L != wordCursorRef.CurrentWord().WordType()) {
            exprContextRef.SetError("Function call missing left bracket of argument list.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }
    isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
    if (ExprNT::IsInFirstSet(wordCursorRef.CurrentWord().WordType())) {
        while (isSuccess) {
            _ExprVec.push_back(new ExprNT());
            isSuccess = isSuccess && _ExprVec.back()->Parse(exprContextRef, wordCursorRef);
            if (isSuccess && WT_Comma == wordCursorRef.CurrentWord().WordType())
                isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
            else
                break;
        }
    }
    if (isSuccess) {
        if (WT_Paranthese_R != wordCursorRef.CurrentWord().WordType()) {
            exprContextRef.SetError("Function call missing right bracket of argument list.", wordCursorRef.CurrentIdx());
            isSuccess = false;
        }
    }
    isSuccess = isSuccess && wordCursorRef.NextWord(exprContextRef);
    isSuccess = isSuccess && OperatorValidate(exprContextRef, funcName, operatorWordIdx);

    return isSuccess;
}

bool FunctionNT::OperatorValidate(ExprContext &exprContextRef, std::string &funcName, int operWordIdx)
{
    vector<int> paramVec;

    for (ExprVecIter_t iter = _ExprVec.begin(); _ExprVec.end() != iter; ++iter)
        paramVec.push_back((*iter)->ResultType());

    FuncParamsInfo funcInfo(funcName.c_str(), paramVec);

    _pExprILCode = FindExprILCode(funcInfo);
    if (NULL != _pExprILCode) {
        return true;
    }
    else {
        exprContextRef.SetError("Cannot find overloaded function for given arguments", operWordIdx);

        return false;
    }
}

ResultTypeEnum FunctionNT::GetResultType(void)
{
    _ASSERT(NULL != _pExprILCode);

    return _pExprILCode->GetReturnType();
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
        _ASSERT(NULL != _pExprILCode);

        _pLeftOne->AppendILSegment(ilSegment);
        _TermNT.AppendILSegment(ilSegment);
        ilSegment.Append(_pExprILCode);
    }

    return ilSegment;
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
        _ASSERT(NULL != _pExprILCode);

        _pLeftOne->AppendILSegment(ilSegment);
        _FactorNT.AppendILSegment(ilSegment);
        ilSegment.Append(_pExprILCode);
    }

    return ilSegment;
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
    else if (NULL != _pFunctionNT) {
        _pFunctionNT->AppendILSegment(ilSegment);
    }
    else {
        _ASSERT(NULL != _pExprILCode);

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

//---------------------------------------------------------------------
// Class member - FunctionNT
//---------------------------------------------------------------------
ExprILCodeSegment& FunctionNT::AppendILSegment(ExprILCodeSegment &ilSegment)
{
    for (ExprVecIter_t iter = _ExprVec.begin(); _ExprVec.end() != iter; ++iter)
        (*iter)->AppendILSegment(ilSegment);
    ilSegment.Append(_pExprILCode);

    return ilSegment;
}