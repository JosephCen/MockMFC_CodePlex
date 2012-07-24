#include "stdafx.h"
#include "WordParser.h"
#include "ExprWorkSpace.h"
#include "ExprILHelper.h"
#include <exception>
#include <string>
#include <sstream>
#include <map>
#include <cctype>
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Class member - WordUnit
//---------------------------------------------------------------------
WordUnit::WordUnit():
_WordType(WT_Invalid)
{ }

WordUnit::WordUnit(WordTypeEnum wordType):
_WordType(wordType)
{
    _ASSERT(WT_Invalid != _WordType);
    _ASSERT(WT_Nul != _WordType);
    _ASSERT(WT_RealValue != _WordType);
}

WordUnit::WordUnit(WordTypeEnum wordType, Matrix::RealVal_t realVal):
_WordType(wordType), _RealVal(realVal)
{
    _ASSERT(WT_RealValue == _WordType);
}

WordUnit::WordUnit(WordTypeEnum wordType, const string &strVal, int intVal) :
_WordType(wordType), _IntVal(intVal), _StrVal(strVal)
{ }

WordUnit::WordUnit(WordTypeEnum wordType, const string &strVal):
_WordType(wordType), _StrVal(strVal)
{ }

string WordUnit::ToString() const
{
    ostringstream resultStrS;

    switch (_WordType) {
        case WT_Invalid :
            resultStrS << "Invalid";
            break;
        case WT_Eof :
            resultStrS << "Eof";
            break;
        case WT_Plus :
            resultStrS << "Plus";
            break;
        case WT_Minus :
            resultStrS << "Minus";
            break;
        case WT_Multiply :
            resultStrS << "Multiply";
            break;
        case WT_Divide :
            resultStrS << "Divide";
            break;
        case WT_DotMultiply :
            resultStrS << "DotMultiply";
            break;
        case WT_Assignment :
            resultStrS << "Assignment";
            break;
        case WT_Paranthese_R :
            resultStrS << "Paranthese_R";
            break;
        case WT_Paranthese_L :
            resultStrS << "Paranthese_L";
            break;
        case WT_S_Bracket_R :
            resultStrS << "S_Bracket_R";
            break;
        case WT_S_Bracket_L :
            resultStrS << "S_Bracket_L";
            break;
        case WT_Comma :
            resultStrS << "Comma";
            break;
        case WT_Semicolon :
            resultStrS << "Semicolon";
            break;
        case WT_RealValue :
            resultStrS << "RealValue" << ":" << _RealVal;
            break;
        case WT_UndefFuncVal :
            resultStrS << "UndefFuncVal" << ":" << _StrVal;
            break;
    }

    return resultStrS.str();
}

//---------------------------------------------------------------------
// Class member - WordParser
//---------------------------------------------------------------------
WordParser::OperatorMap_t WordParser::s_OperatorMap;
bool WordParser::s_IsInitialized = false;
WordParser* WordParser::s_pDefaultParser = NULL;

void WordParser::Initialize()
{
    if (!s_IsInitialized) {
        // Initialize 's_OperatorMap'
        s_OperatorMap["+"] = WT_Plus;
        s_OperatorMap["-"] = WT_Minus;
        s_OperatorMap["*"] = WT_Multiply;
        s_OperatorMap["/"] = WT_Divide;
        s_OperatorMap[".*"] = WT_DotMultiply;
        s_OperatorMap["./"] = WT_DotDivide;
        s_OperatorMap["="] = WT_Assignment;
        s_OperatorMap[")"] = WT_Paranthese_R;
        s_OperatorMap["("] = WT_Paranthese_L;
        s_OperatorMap["]"] = WT_S_Bracket_R;
        s_OperatorMap["["] = WT_S_Bracket_L;
        s_OperatorMap[","] = WT_Comma;
        s_OperatorMap[";"] = WT_Semicolon;

        s_IsInitialized = true;
    }
}

WordParser* WordParser::GetDefaultParserPtr()
{
    if (NULL == s_pDefaultParser)
        s_pDefaultParser = new WordParser();

    return s_pDefaultParser;
}

WordParser::WordParser(void):
_pCurWorkSpace(nullptr)
{ }

WordFwCursor WordParser::GenWordFwCursor(const string &inputStr)
{
    return WordFwCursor(this, inputStr);
}

WordFwCursor* WordParser::GenWordFwCursorPtr(const std::string &inputStr)
{
    return new WordFwCursor(this, inputStr);
}

bool WordParser::NextWord(WordFwCursor &wordCursorRef)
{
    WordUnit newWord;

    if (!wordCursorRef.IsEof())
    {
        StrIter_t startIter = wordCursorRef.GetCurStrIter();
        StrIter_t endIter = wordCursorRef.GetCurStrIter();
        StrIter_t eofIter = wordCursorRef.GetCurInputStr().end();

        while (startIter != eofIter && isspace(*startIter))
            ++startIter;
        if (startIter != eofIter) {
            switch (GetNextWordEndIter(startIter, eofIter, endIter)) {
                case NST_Operator :
                    if (ParseOperator(string(startIter, endIter), newWord))
                        break;
                case NST_OperatorFail :
                    wordCursorRef.SetError(startIter, "Fail to parse a operator");
                    break;
                case NST_FuncVar :
                    if (ParseFuncVar(string(startIter, endIter), newWord))
                        break;
                case NST_FuncVarFail :
                    wordCursorRef.SetError(startIter, "Fail to parse a function or variable");
                    break;
                case NST_RealVal :
                    if (strchr("+-", *startIter) && wordCursorRef.CurrentWord() && (strchr("])", *(startIter - 1)) || WT_RealValue == wordCursorRef.CurrentWord().WordType())) {
                        endIter = startIter + 1;
                        ParseOperator(string(startIter, endIter), newWord);
                        break;
                    }
                    else {
                        if (ParseRealVal(string(startIter, endIter), newWord))
                            break;
                    }
                case NST_RealValFail :
                    wordCursorRef.SetError(startIter, "Fail to parse a real value");
                    break;
                case NST_UnknowFail :
                    wordCursorRef.SetError(startIter, "A unknow parse fail happend");
                    break;
            }

            if ((bool)wordCursorRef) {
                wordCursorRef.SetCurWordUnit(newWord);
                wordCursorRef.SetCurStrIter(endIter);
            }
        }
        else {
            newWord = WordUnit(WT_Eof);
            wordCursorRef.SetCurWordUnit(newWord);
            wordCursorRef.SetCurStrIter(eofIter);
        }
    }
    

    return (bool)newWord;
}

//void WordParser::SetError(StrIter_t iter, const char *errorCh)
//{
//    _ASSERT(!_IsErrorState);
//
//    _IsErrorState = true;
//    _pExprEx = new ExprException(errorCh, static_cast<int>(iter - _InputStr.begin()));
//}

bool WordParser::ParseRealVal(const string &str, WordUnit &wordRef)
{
    _ASSERT(str.length() != 0);
    
    if (NULL == strpbrk(str.c_str(), "123456789")) {
        wordRef = WordUnit(WT_RealValue, 0.0);

        return true;
    }
    else {
        Matrix::RealVal_t realVal = atof(str.c_str());
        if (0.0 != realVal) {
            wordRef = WordUnit(WT_RealValue, realVal);

            return true;
        }
        else {
            return false;
        }
    }
}

bool WordParser::ParseFuncVar(const string &str, WordUnit &wordRef)
{
    _ASSERT(str.length() != 0);

    // TODO: Parse a defined function or variable
    if (NULL != _pCurWorkSpace) {
		Variable_sp spVariable;

        if ((bool)(spVariable = _pCurWorkSpace->_GlobalVarSet.SearchVar(str)))
            wordRef = WordUnit(WT_DefVariable, str, spVariable->GetTypeId());
        else if (ExprILHelper::FindOperatorILCount(str)) // TODO: Parse a defined function which's scope is current WorkSapce
            wordRef = WordUnit(WT_DefFunction, str);
        else
            wordRef = WordUnit(WT_UndefFuncVal, str);
    }
    else
        wordRef = WordUnit(WT_UndefFuncVal, str);

    return true;
}

bool WordParser::ParseOperator(const string &str, WordUnit &wordRef)
{
    _ASSERT(str.length() != 0);

    if (GetOperatorMap().count(str)) {
        wordRef = WordUnit(GetOperatorMap()[str]);

        return true;
    }

    return false;
}

NextStrTypeEnum WordParser::GetNextWordEndIter(StrIter_t iter1, StrIter_t iter2, StrIter_t &endIterRef)
{
    NextStrTypeEnum strType = NST_UnknowFail;
    struct
    {
        char state;
        StrIter_t iter;
    } current; //, accepted;

    _ASSERT(!isspace(*iter1));
    switch (*iter1) {
        case '+' :
        case '-' :
            current.state = 'B';
            break;
        case '0' :
            current.state = 'C';
            break;
        case '*' :
        case '/' :
        case '=' :
        case '(' :
        case ')' :
        case '[' :
        case ']' :
        case ',' :
        case ';' :
            current.state = 'G';
            break;
        case '.' :
            current.state = 'O';
            break;
        default :
            if (isalpha(*iter1))
                current.state = 'E';
            else if (isdigit(*iter1)) // It should be [1-9]. But 0 has been catch by "case '0'", so it is ok.
                current.state = 'D';
            else
                current.state = '\0';
            break;
    }
    current.iter = iter1 + 1;

    while (current.iter != iter2) {
        switch (current.state) {
            case 'B' :
                if (*current.iter == '0')
                    current.state = 'C';
                else if (isdigit(*current.iter))
                    current.state = 'D';
                else
                    goto EndMatch;
                break;
            case 'C' :
                if (*current.iter == '.')
                    current.state = 'J';
                else
                    goto EndMatch;
                break;
            case 'D' :
                if (isdigit(*current.iter))
                    current.state = 'D';
                else if (*current.iter == '.')
                    current.state = 'J';
                else
                    goto EndMatch;
                break;
            case 'E' :
                if (isalnum(*current.iter))
                    current.state = 'E';
                else
                    goto EndMatch;
                break;
            case 'F' :
                _ASSERT(0);
                break;
            case 'G' :
                goto EndMatch;
            case 'H' :
                _ASSERT(0);
                break;
            case 'J' :
                if (isdigit(*current.iter))
                    current.state = 'M';
                else
                    goto EndMatch;
                break;
            case 'K' :
                _ASSERT(0);
                break;
            case 'L' :
                _ASSERT(0);
                break;
            case 'M' :
                if (isdigit(*current.iter))
                    current.state = 'M';
                else
                    goto EndMatch;
                break;
            case 'N' :
                _ASSERT(0);
                break;
            case 'O' :
                if (strchr("*/", *current.iter))
                    current.state = 'G';
                else
                    goto EndMatch;
                break;
            default :
                goto EndMatch;
                break;
        }
        ++current.iter;
    }

EndMatch :
    switch (current.state) {
        case 'B' :
        case 'G' :
            strType = NST_Operator;
            break;
        case 'L' :
        case 'O' :
            strType = NST_OperatorFail;
            break;
        case 'C' :
            if (current.iter == iter2 || !isdigit(*current.iter))
                strType = NST_RealVal;
            else
                strType = NST_RealValFail;
            break;
        case 'D' :
        case 'M' :
            strType = NST_RealVal;
            break;
        case 'J' :
            strType = NST_RealValFail;
            break;
        case 'E' :
            strType = NST_FuncVar;
            break;
        default :
            strType = NST_UnknowFail;
            break;
    }
    endIterRef = current.iter;

    return strType;
}

//---------------------------------------------------------------------
// Class member - WordFwCursor
//---------------------------------------------------------------------
WordFwCursor::WordFwCursor(WordParser *pParser, const string &inputStr) :
_pParser(pParser), _InputStr(inputStr), _IsErrorState(false), _pExprEx(NULL)
{
    _CurStrIter = _InputStr.begin();
}

WordFwCursor::WordFwCursor(const WordFwCursor &wordCursor) :
_pParser(wordCursor._pParser), _InputStr(wordCursor._InputStr), _IsErrorState(wordCursor._IsErrorState),
_pExprEx(NULL),_CurStrIter(wordCursor._CurStrIter), _CurWordUnit(wordCursor._CurWordUnit)
{
    if (NULL != wordCursor._pExprEx)
        _pExprEx = new ExprException(*wordCursor._pExprEx);
}

WordFwCursor::~WordFwCursor()
{
    if (NULL != _pExprEx)
        delete _pExprEx;
}

void WordFwCursor::SetError(const WordFwCursor::StrIter_t &strIter, const char *errorCh)
{
    _ASSERT(!_IsErrorState);

    _IsErrorState = true;
    _pExprEx = new ExprException(errorCh, static_cast<int>(strIter - _InputStr.begin()));
}

bool WordFwCursor::NextWord(ExprContext &exprContextRef)
{
    if (!NextWord()) {
        exprContextRef.SetError(*_pExprEx);
        
        return false;
    }

    return true;
}

string WordFwCursor::ErrorMsg() const
{
    _ASSERT(_IsErrorState);

    ostringstream errMsgStrS;

    errMsgStrS << "Error at[" << _pExprEx->ErrorChIdx() << "]: " << _pExprEx->what();

    return errMsgStrS.str();
}

WordFwCursor& WordFwCursor::operator =(const WordFwCursor &wordCursor)
{
    if (this != &wordCursor)
    {
        _pParser = wordCursor._pParser;
        _InputStr = wordCursor._InputStr;
        _CurStrIter = wordCursor._CurStrIter;
        _CurWordUnit = wordCursor._CurWordUnit;
        _IsErrorState = wordCursor._IsErrorState;

        if (NULL != _pExprEx)
        {
            delete _pExprEx;
            _pExprEx = NULL;
        }

        if (NULL != wordCursor._pExprEx)
            _pExprEx = new ExprException(*wordCursor._pExprEx);
    }

    return *this;
}