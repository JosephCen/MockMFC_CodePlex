#ifndef NonTerminal_H
#define NonTerminal_H

#include <list>
#include "WordParser.h"
#include "ExprILCode.h"
#include "ExprWorkSpace.h"
#include "Matrix.h"
#include "ExprException.h"
#include "ExprAdapter.h"
#include "FunctionSet.h"
#include <crtdbg.h>

//---------------------------------------------------------------------
// BaseNonTerminal - class
//---------------------------------------------------------------------
class BaseNonTerminal
{
private :
    ResultTypeEnum _ResultType;
public :
    // Static Methods
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef) = 0;
    ResultTypeEnum ResultType(void);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment) = 0;
protected :
    // Constructor
    BaseNonTerminal(): _ResultType(RT_None) { }
    // Methods
    virtual ResultTypeEnum GetResultType(void) { _ASSERT(0); return RT_None; }
    ExprILCode* FindExprILCode(const FuncParamsInfo &funcInfo);
    ExprILCode* FindExprILCode(WordTypeEnum operWordType, ResultTypeEnum lParamType, ResultTypeEnum rParamType);
};

//---------------------------------------------------------------------
// ExprNT - class declare
//---------------------------------------------------------------------
class ExprNT;
//---------------------------------------------------------------------
// ListNT - class
//---------------------------------------------------------------------
class ListNT : public BaseNonTerminal
{
private :
    typedef std::list<ExprNT*> ExprList_t;
    ExprList_t _ExprList;
    bool _IsEndWithSemicolon;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    ListNT(void): _IsEndWithSemicolon(true), _ExprList(), BaseNonTerminal() { }
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    ~ListNT();
};

//---------------------------------------------------------------------
// StartNT - class
//---------------------------------------------------------------------
class StartNT : public BaseNonTerminal
{
private :
    ListNT _ListNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    StartNT(void): _ListNT(), BaseNonTerminal() { }
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
};

//---------------------------------------------------------------------
// SubExprNT - class declare
//---------------------------------------------------------------------
class SubExprNT;
//---------------------------------------------------------------------
// ExprNT - class
//---------------------------------------------------------------------
class ExprNT : public BaseNonTerminal
{
private :
    SubExprNT *_pSubExprNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    ExprNT(void): _pSubExprNT(NULL), BaseNonTerminal() { }
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    ~ExprNT();
protected :
    virtual ResultTypeEnum GetResultType(void);
};

//---------------------------------------------------------------------
// SubTermNT - class declare
//---------------------------------------------------------------------
class SubTermNT;
//---------------------------------------------------------------------
// TermNT - class
//---------------------------------------------------------------------
class TermNT : public BaseNonTerminal
{
private :
    SubTermNT *_pSubTermNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    TermNT(void): _pSubTermNT(NULL), BaseNonTerminal() { }
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    ~TermNT();
protected :
    virtual ResultTypeEnum GetResultType();
};

//---------------------------------------------------------------------
// SubExprNT - class
//---------------------------------------------------------------------
class SubExprNT : public BaseNonTerminal
{
private :
    bool _IsFirstOne;
    ExprILCode *_pExprILCode;
    TermNT _TermNT;
    SubExprNT *_pLeftOne;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType, bool isFirstOne);
    // Constructor
    SubExprNT(void);
    SubExprNT(SubExprNT *pLeftOne);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    ~SubExprNT();
private :
    bool OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx);
protected :
    virtual ResultTypeEnum GetResultType(void);
};

//---------------------------------------------------------------------
// MatrixNT - class declare
//---------------------------------------------------------------------
class MatrixNT;
//---------------------------------------------------------------------
// FactorNT - class
//---------------------------------------------------------------------
class FactorNT : public BaseNonTerminal
{
private :
    ResultTypeEnum _ResultType;
    ExprNT *_pExprNT;
    MatrixNT *_pMatrixNT;
    ExprILCode *_pExprILCode;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    FactorNT(void): _ResultType(RT_None), _pExprNT(NULL), _pMatrixNT(NULL), _pExprILCode(NULL), BaseNonTerminal() { }
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    ~FactorNT();
protected :
    virtual ResultTypeEnum GetResultType(void);
};

//---------------------------------------------------------------------
// SubTermNT - class
//---------------------------------------------------------------------
class SubTermNT : public BaseNonTerminal
{
private :
    bool _IsFirstOne;
    ExprILCode *_pExprILCode;
    FactorNT _FactorNT;
    SubTermNT *_pLeftOne;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType, bool isFirstOne);
    // Constructor
    SubTermNT(void);
    SubTermNT(SubTermNT *pLeftOne);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    ~SubTermNT();
private :
    bool OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx);
protected :
    virtual ResultTypeEnum GetResultType(void);
};

//---------------------------------------------------------------------
// MatrixColNT - class
//---------------------------------------------------------------------
class MatrixColNT : public BaseNonTerminal
{
private :
    ExprNT _ExprNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    MatrixColNT(void): _ExprNT(), BaseNonTerminal() { }
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
protected :
    virtual ResultTypeEnum GetResultType(void) { return _ExprNT.ResultType(); }
};

//---------------------------------------------------------------------
// MatrixColsNT - class
//---------------------------------------------------------------------
class MatrixColsNT : public BaseNonTerminal
{
private :
    typedef std::list<MatrixColNT*> ColList_t;
    ColList_t _ColList;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    MatrixColsNT(void): _ColList(), BaseNonTerminal() { }
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    Matrix::Row_Col_t Cols() const { return _ColList.size(); }
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    ~MatrixColsNT();
private :
    MatrixColsNT(const MatrixColsNT &matrixColsNTRef);
};

//---------------------------------------------------------------------
// MatrixRowsNT - class
//---------------------------------------------------------------------
class MatrixRowsNT : public BaseNonTerminal
{
private :
    typedef std::list<MatrixColsNT*> RowList_t;
    RowList_t _RowList;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    MatrixRowsNT(void): _RowList(), BaseNonTerminal() { }
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    Matrix::Row_Col_t Rows() const { return _RowList.size(); }
    Matrix::Row_Col_t Cols() const { return _RowList.front()->Cols(); }
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    ~MatrixRowsNT();
private :
    MatrixRowsNT(const MatrixRowsNT &matrixRowsNTRef);
};

//---------------------------------------------------------------------
// MatrixNT - class
//---------------------------------------------------------------------
class MatrixNT : public BaseNonTerminal
{
private :
    MatrixRowsNT _MatrixRows;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    MatrixNT(void): _MatrixRows(), BaseNonTerminal() { }
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    Matrix::Row_Col_t Rows() const { return _MatrixRows.Rows(); }
    Matrix::Row_Col_t Cols() const { return _MatrixRows.Cols(); }
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
protected :
    virtual ResultTypeEnum GetResultType(void) { return RT_Matrix; }
};

#endif