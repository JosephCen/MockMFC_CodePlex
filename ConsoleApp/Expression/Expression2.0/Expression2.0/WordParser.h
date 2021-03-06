#ifndef WordParser_H
#define WordParser_H

#include <string>
#include <map>
#include <crtdbg.h>
#include "ClsPreDeclaration.h"
#include "Matrix.h"
#include "ExprAdapter.h"
#include "ExprException.h"

enum WordTypeEnum
{
   WT_Invalid = -1,
   WT_Nul,
   WT_Eof,
   WT_Plus,          // +
   WT_Minus,         // -
   WT_Multiply,      // *
   WT_Divide,        // /
   WT_DotMultiply,   // .*
   WT_DotDivide,     // ./
   WT_Negative,      
   WT_Paranthese_R,  // )
   WT_Paranthese_L,  // (
   WT_S_Bracket_R,   // ]
   WT_S_Bracket_L,   // [
   WT_Comma,         // ,
   WT_Semicolon,     // ;
   WT_RealValue,     // [123.23]
   WT_UndefFuncVal,  // [a[a-1..]]
   WT_DefVariable,
   WT_DefFunction
};

//---------------------------------------------------------------------
// WordUnit - class
//---------------------------------------------------------------------
class WordUnit
{
private :
    WordTypeEnum _WordType;
    Matrix::RealVal_t _RealVal;
    std::string _StrVal;
public :
    // Constructs
    WordUnit();
    explicit WordUnit(WordTypeEnum wordType);
    WordUnit(WordTypeEnum wordType, Matrix::RealVal_t realVal);
    WordUnit(WordTypeEnum wordType, const std::string &strVal);
    // Methods
    WordTypeEnum WordType(void) const { return _WordType; }
    Matrix::RealVal_t RealValue(void) const;
    const std::string& StringValue(void) const;
    std::string ToString() const;
    // Override Operators
    operator bool() const { return WT_Invalid != _WordType; }
};

//---------------------------------------------------------------------
// Inline methods - WordUnit
//---------------------------------------------------------------------
inline Matrix::RealVal_t WordUnit::RealValue(void) const
{
    _ASSERT(WT_RealValue == _WordType);

    return _RealVal;
}

inline const std::string& WordUnit::StringValue(void) const
{
    return _StrVal;
}


enum NextStrTypeEnum
{
    NST_UnknowFail,
    NST_Operator,
    NST_OperatorFail,
    NST_FuncVar,
    NST_FuncVarFail,
    NST_RealVal,
    NST_RealValFail
};

//---------------------------------------------------------------------
// WordParser - class
//---------------------------------------------------------------------
class WordParser
{
private :
    typedef std::string::const_iterator StrIter_t;
    typedef std::map<std::string, WordTypeEnum> OperatorMap_t;

    static OperatorMap_t s_OperatorMap;
    static bool s_IsInitialized;
    static WordParser *s_pDefaultParser;

    ExprWorkSpace *_pCurExprWS;
public :
    // Constructs
    WordParser(void);
    // Methods
    void SetExprWorkSpace(ExprWorkSpace *pExprWorkSpace);
    WordFwCursor GenWordFwCursor(const std::string &inputStr);
    WordFwCursor* GenWordFwCursorPtr(const std::string &inputStr);
    // Static Methods
    static WordParser* GetDefaultParserPtr();
private :
    // Static Methods
    static void Initialize();
    // Methods
    NextStrTypeEnum GetNextWordEndIter(StrIter_t iter1, StrIter_t iter2, StrIter_t &endIterRef);
    bool ParseRealVal(const std::string &str, WordUnit &wordRef);
    bool ParseFuncVar(const std::string &str, WordUnit &wordRef);
    bool ParseOperator(const std::string &str, WordUnit &wordRef);
    static OperatorMap_t& GetOperatorMap();
    bool NextWord(WordFwCursor &wordCursorRef);
    // Friend declaration
    friend class WordFwCursor;
};

//---------------------------------------------------------------------
// Inline methods - WordParser
//---------------------------------------------------------------------
inline void WordParser::SetExprWorkSpace(ExprWorkSpace *pExprWorkSpace)
{
    _pCurExprWS = pExprWorkSpace;
}

inline WordParser::OperatorMap_t& WordParser::GetOperatorMap() 
{
    if (!s_IsInitialized)
        Initialize();

    return s_OperatorMap;
}

//---------------------------------------------------------------------
// WordFwCursor - class
//---------------------------------------------------------------------
class WordFwCursor
{
friend class WordParser;
public :
    typedef std::string::const_iterator StrIter_t;
private :
    WordParser *_pParser;
    std::string _InputStr;
    StrIter_t _CurStrIter;
    WordUnit _CurWordUnit;
    // Error relative member
    bool _IsErrorState;
    ExprException *_pExprEx;
private :
    // Constructor
    WordFwCursor(WordParser *pParser, const std::string &inputStr);
    // Methods
    void SetCurStrIter(const StrIter_t &strIter);
    void SetCurWordUnit(const WordUnit &wordUnit);
    void SetError(const StrIter_t &strIter, const char *errorCh);
public :
    // Constructor
    WordFwCursor(const WordFwCursor &wordCursor);
    // Destructor
    ~WordFwCursor();
    // Methods
    StrIter_t GetCurStrIter() const;
    const std::string& GetCurInputStr() const;
    WordUnit CurrentWord() const;
    bool IsEof() const;
    int CurrentIdx() const;
    bool NextWord();
    bool NextWord(ExprContext &exprContextRef);
    std::string ErrorMsg() const;
    // Override Operators
    operator bool() const;
    WordFwCursor& operator =(const WordFwCursor &wordCursor);
};

//---------------------------------------------------------------------
// Inline methods - WordFwCursor
//---------------------------------------------------------------------
inline void WordFwCursor::SetCurStrIter(const WordFwCursor::StrIter_t &strIter)
{
    _CurStrIter = strIter;
}

inline WordParser::StrIter_t WordFwCursor::GetCurStrIter() const
{
    return _CurStrIter;
}

inline void WordFwCursor::SetCurWordUnit(const WordUnit &wordUnit)
{
    _CurWordUnit = wordUnit;
}

inline const std::string& WordFwCursor::GetCurInputStr() const
{
    return _InputStr;
}

inline WordUnit WordFwCursor::CurrentWord() const
{
    return _CurWordUnit;
}

inline bool WordFwCursor::IsEof() const
{
    return WT_Eof == _CurWordUnit.WordType();
}

inline int WordFwCursor::CurrentIdx() const
{
    return static_cast<int>(_CurStrIter - _InputStr.begin());
}

inline bool WordFwCursor::NextWord()
{
    return _pParser->NextWord(*this);
}

inline bool WordFwCursor::NextWord(ExprContext &exprContextRef)
{
    if (!NextWord())
    {
        exprContextRef.SetError(*_pExprEx);
        
        return false;
    }

    return true;
}

inline WordFwCursor::operator bool() const
{
    return !_IsErrorState;
}

#endif