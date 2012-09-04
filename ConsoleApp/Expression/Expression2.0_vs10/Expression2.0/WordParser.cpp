#include "stdafx.h"
#include "WordParser.h"
#include "ExprWorkSpace.h"
#include "ExprILHelper.h"
#include <algorithm>
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
        case WT_UndefFunction :
            resultStrS << "UndefFunction" << ":" << _StrVal;
            break;
        case WT_UndefVariable :
            resultStrS << "UndefVariable" << ":" << _StrVal;
            break;
    }

    return resultStrS.str();
}

//---------------------------------------------------------------------
// Class member - WordParser
//---------------------------------------------------------------------
#ifdef _DEBUG
int WordParser::s_InstanceCount = 0;
#endif

WordParser::OperatorMap_t WordParser::s_OperatorMap;
bool WordParser::s_IsInitialized = false;
WordParser* WordParser::s_pDefaultParser = NULL;

NextStrTypeEnum WordParser::s_NextStrTypeArr[] = {
    NST_UnknowFail,
    NST_RealVal,
    NST_HexInt,
    NST_Operator,
    NST_Function,
    NST_Variable
};
const char* WordParser::s_WordRegexPat = "\\s*(?:"
    "((?:[1-9]\\d*|0(?![xX]))(?:\\.\\d+)?)|" // Use negative lookahead to avoid 0 of a hexadecimal (i.e. 0 of 0x1) captured by this patten.
    "(0[xX][[:xdigit:]]+)|"
    "([+\\-*/=\\(\\)\\[\\],;]|\\.[*/])|"
    "([[:alpha:]]\\w*)(?=\\s*\\()|" // A left paranthese '(' is expected after a name of a function.
    "([[:alpha:]]\\w*)"
    ")";
regex WordParser::s_WordRegex;

void WordParser::ReleaseRes()
{
    _ASSERT_EXPR(0 == s_InstanceCount, L"Before call this function, all instance of WordParse should have been destructed!");
    s_OperatorMap.clear();
    s_WordRegex.assign("");
    s_IsInitialized = false;
}

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

        // Initialize 's_WordRegex'
        s_WordRegex.assign(s_WordRegexPat);

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
{
    if (!s_IsInitialized)
        Initialize();
#ifdef _DEBUG
    ++s_InstanceCount;
#endif
}

WordParser::~WordParser()
{
#ifdef _DEBUG
    --s_InstanceCount;
#endif
}

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

    if (!wordCursorRef.IsEof()) {
        const sregex_iterator endRegexIter;

        if (endRegexIter != wordCursorRef._RegexIter) {
            const smatch &m = *wordCursorRef._RegexIter;
            unsigned i = 1; 

            while (!m[i].matched)
                ++i;

            switch (s_NextStrTypeArr[i]) {
                case NST_Operator :
                    if (!ParseOperator(m[i].str(), newWord))
                        wordCursorRef.SetError(wordCursorRef._StrPos, "Fail to parse a operator");
                    break;
                case NST_Function :
                    if (!ParseFunction(m[i].str(), newWord))
                        wordCursorRef.SetError(wordCursorRef._StrPos, "Fail to parse a function");
                    break;
                case NST_Variable :
                    if (!ParseVariable(m[i].str(), newWord))
                        wordCursorRef.SetError(wordCursorRef._StrPos, "Fail to parse a variable");
                    break;
                case NST_RealVal :
                    if (!ParseRealVal(m[i].str(), newWord))
                        wordCursorRef.SetError(wordCursorRef._StrPos, "Fail to parse a real value");
                    break;
                case NST_HexInt :
                    if (!ParseHexInt(m[i].str(), newWord))
                        wordCursorRef.SetError(wordCursorRef._StrPos, "Fail to parse a Hex integer value");
                    break;
            }

            if ((bool)newWord) {
                wordCursorRef.SetCurWordUnit(newWord);

                wordCursorRef._StrPos += m[0].length();
                ++wordCursorRef._RegexIter;
            }
        }
        else {
            if (wordCursorRef._InputStr.length() == wordCursorRef._StrPos) {
                newWord = WordUnit(WT_Eof);
                wordCursorRef.SetCurWordUnit(newWord);
            }
            else {
                wordCursorRef.SetError(wordCursorRef._StrPos, "A unknow parse fail happend");
            }
        }
    }

    return (bool)newWord;
}

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

bool WordParser::ParseHexInt(const std::string &str, WordUnit &wordRef)
{
    _ASSERT(str.length() != 0);

    if (nullptr == strpbrk(str.c_str(), "123456789abcdefABCDEF")) {
        wordRef = WordUnit(WT_RealValue, 0.0);

        return true;
    }
    else {
        unsigned __int64 lInt = _strtoui64(str.c_str() + 2, nullptr, 16); // '+ 2' is used to elimite the prefix 0x.

        if (_UI64_MAX != lInt) {
            wordRef = WordUnit(WT_RealValue, static_cast<Matrix::RealVal_t>(lInt));

            return true;
        }
        else {
            return false;
        }
    }
}

bool WordParser::ParseFunction(const string &str, WordUnit &wordRef)
{
    _ASSERT(str.length() != 0);

    if (nullptr != _pCurWorkSpace) {
        if (ExprILHelper::FindOperatorILCount(str) > 0) // TODO: Parse a defined function which's scope is current WorkSapce
            wordRef = WordUnit(WT_DefFunction, str);
        else
            wordRef = WordUnit(WT_UndefFunction, str);
    }
    else
        wordRef = WordUnit(WT_UndefFunction, str);

    return true;
}

bool WordParser::ParseVariable(const string &str, WordUnit &wordRef)
{
    _ASSERT(str.length() != 0);

    if (NULL != _pCurWorkSpace) {
		Variable_sp spVariable;

        if ((bool)(spVariable = _pCurWorkSpace->_GlobalVarSet.SearchVar(str)))
            wordRef = WordUnit(WT_DefVariable, str, spVariable->GetTypeId());
        else
            wordRef = WordUnit(WT_UndefVariable, str);
    }
    else
        wordRef = WordUnit(WT_UndefVariable, str);

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

//---------------------------------------------------------------------
// Class member - WordFwCursor
//---------------------------------------------------------------------
WordFwCursor::WordFwCursor(WordParser *pParser, const string &inputStr) :
    _pParser(pParser), _InputStr(inputStr), _IsErrorState(false), _pExprEx(NULL),
    _RegexIter(inputStr.begin(), inputStr.end(), WordParser::s_WordRegex, regex_constants::match_continuous),
    _StrPos(0)
{
    // rtrim _InputStr
    auto p = find_if(_InputStr.rbegin(), _InputStr.rend(), [](string::value_type ch) -> bool {
        return !isspace(ch);
    });
    _InputStr.erase(p.base(), _InputStr.end());
}

WordFwCursor::WordFwCursor(const WordFwCursor &wordCursor) :
    _pParser(wordCursor._pParser), _InputStr(wordCursor._InputStr), _IsErrorState(wordCursor._IsErrorState),
    _pExprEx(NULL), _CurWordUnit(wordCursor._CurWordUnit),
    _StrPos(wordCursor._StrPos), _RegexIter(wordCursor._RegexIter)
{
    if (NULL != wordCursor._pExprEx)
        _pExprEx = new ExprException(*wordCursor._pExprEx);
}

WordFwCursor::~WordFwCursor()
{
    if (NULL != _pExprEx)
        delete _pExprEx;
}

void WordFwCursor::SetError(size_t strPos, const char *errorCh)
{
    _ASSERT(!_IsErrorState);

    _IsErrorState = true;
    _pExprEx = new ExprException(errorCh, static_cast<int>(strPos));
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
        _StrPos = wordCursor._StrPos;
        _RegexIter = wordCursor._RegexIter;
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