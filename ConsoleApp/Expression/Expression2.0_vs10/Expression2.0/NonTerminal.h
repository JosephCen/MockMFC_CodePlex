#ifndef NonTerminal_H
#define NonTerminal_H

#include <list>
#include <vector>
#include <memory>
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
	// Destructor
    virtual ~BaseNonTerminal() { }
protected :
    // Constructor
    BaseNonTerminal(void);
    BaseNonTerminal(BaseNonTerminal &&rValRef);
    // Methods
    virtual ResultTypeEnum GetResultType(void) { _ASSERT(0); return RT_None; }
    ExprILCode_sp FindExprILCode(const FuncParamsInfo &funcInfo);
    ExprILCode_sp FindExprILCode(WordTypeEnum operWordType, ResultTypeEnum lParamType, ResultTypeEnum rParamType);
};

//---------------------------------------------------------------------
// class declare
//---------------------------------------------------------------------
class StartNT;

class ListNT;

class CodeLineNT;

class AssignExprNT;

class ExprNT;

class SubExprNT;

class TermNT;

class SubTermNT;

class FactorNT;

class MatrixNT;

class FunctionNT;

//---------------------------------------------------------------------
// ListNT - class
//---------------------------------------------------------------------
class ListNT : public BaseNonTerminal
{
private :
    typedef std::vector<CodeLineNT> CodeList_t;
    CodeList_t _CodeList;
    bool _IsEndWithSemicolon;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    ListNT(void);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
private :
    // Constructor (Do not allow copy construct)
    ListNT(const ListNT&);
    // Assignment operator (Do not allow assignment operator)
    ListNT& operator=(const ListNT&);
};

//---------------------------------------------------------------------
// CodeLineNT - class
//---------------------------------------------------------------------
class CodeLineNT : public BaseNonTerminal
{
private :
	BaseNonTerminal *_pInnerNT;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    CodeLineNT(void);
    CodeLineNT(CodeLineNT &&rValRef);
    // Destructor
    virtual ~CodeLineNT();
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
private :
    // Constructor (Do not allow copy construct)
    CodeLineNT(const CodeLineNT&);
    // Assignment operator (Do not allow assignment operator)
    CodeLineNT& operator=(const CodeLineNT&);
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
    virtual ~ExprNT();
protected :
    virtual ResultTypeEnum GetResultType(void);
private :
    // Constructor (Do not allow copy construct)
    ExprNT(const ExprNT&);
	// Assignment operator (Do not allow assignment operator)
	ExprNT& operator=(const ExprNT&);
};

//---------------------------------------------------------------------
// AssignExprNT - class
//---------------------------------------------------------------------
class AssignExprNT : public BaseNonTerminal
{
private :
    enum Flag
    {
        F_None,
        F_JustExpr,
        F_UnDefVar,
        F_DefinedVar,
    };

    Flag _Flag;
    ExprILCode_sp _spExprILCode;
    ExprNT _Expr;
    AssignExprNT *_pRightOne;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    AssignExprNT(void);
    // Destructor
    virtual ~AssignExprNT();
	// Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
protected :
    virtual ResultTypeEnum GetResultType();
private :
    // Constructor (Do not allow copy construct)
    AssignExprNT(const AssignExprNT&);
	// Assignment operator (Do not allow assignment operator)
	AssignExprNT& operator=(const AssignExprNT&);
};

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
    virtual ~TermNT();
protected :
    virtual ResultTypeEnum GetResultType();
private :
    // Constructor (Do not allow copy construct)
    TermNT(const TermNT&);
	// Assignment operator (Do not allow assignment operator)
	TermNT& operator=(const TermNT&);
};

//---------------------------------------------------------------------
// SubExprNT - class
//---------------------------------------------------------------------
class SubExprNT : public BaseNonTerminal
{
private :
    bool _IsFirstOne;
    ExprILCode_sp _spExprILCode;
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
    virtual ~SubExprNT();
private :
    bool OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx);
protected :
    virtual ResultTypeEnum GetResultType(void);
private :
    // Constructor (Do not allow copy construct)
    SubExprNT(const SubExprNT&);
	// Assignment operator (Do not allow assignment operator)
	SubExprNT& operator=(const SubExprNT&);
};

//---------------------------------------------------------------------
// FactorNT - class
//---------------------------------------------------------------------
class FactorNT : public BaseNonTerminal
{
private :
	BaseNonTerminal *_pInnerNT;
    ExprILCode_sp _spExprILCode;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    FactorNT(void);
	// Destructor
    virtual ~FactorNT();
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
protected :
    virtual ResultTypeEnum GetResultType(void);
private :
    // Constructor (Do not allow copy construct)
    FactorNT(const FactorNT&);
	// Assignment operator (Do not allow assignment operator)
	FactorNT& operator=(const FactorNT&);
};

//---------------------------------------------------------------------
// SubTermNT - class
//---------------------------------------------------------------------
class SubTermNT : public BaseNonTerminal
{
private :
    bool _IsFirstOne;
    ExprILCode_sp _spExprILCode;
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
    virtual ~SubTermNT();
private :
    bool OperatorValidate(ExprContext &exprContextRef, WordTypeEnum operWordType, int operWordIdx);
protected :
    virtual ResultTypeEnum GetResultType(void);
private :
    // Constructor (Do not allow copy construct)
    SubTermNT(const SubTermNT&);
	// Assignment operator (Do not allow assignment operator)
	SubTermNT& operator=(const SubTermNT&);
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
    virtual ~MatrixColsNT();
private :
	// Constructor (Do not allow copy construct)
    MatrixColsNT(const MatrixColsNT&);
	// Assignment operator (Do not allow assignment operator)
	MatrixColsNT& operator=(const MatrixColsNT&);
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
    virtual ~MatrixRowsNT();
private :
	// Constructor (Do not allow copy construct)
    MatrixRowsNT(const MatrixRowsNT&);
	// Assignment operator (Do not allow assignment operator)
	MatrixRowsNT& operator=(const MatrixRowsNT&);
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

//---------------------------------------------------------------------
// FunctionNT - class
//---------------------------------------------------------------------
class FunctionNT : public BaseNonTerminal
{
private :
    typedef std::vector<ExprNT*> ExprVec_t;
    typedef std::vector<ExprNT*>::iterator ExprVecIter_t;
    typedef std::vector<ExprNT*>::const_iterator ExprVecCIter_t;
    ExprVec_t _ExprVec;
    ExprILCode_sp _spExprILCode;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    FunctionNT(void);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
	// Destructor
    virtual ~FunctionNT();
private :
    bool OperatorValidate(ExprContext &exprContextRef, std::string &funcName, int operWordIdx);
protected :
    virtual ResultTypeEnum GetResultType(void);
private :
    // Constructor (Do not allow copy construct)
    FunctionNT(const FunctionNT&);
	// Assignment operator (Do not allow assignment operator)
	FunctionNT& operator=(const FunctionNT&);
};

#endif