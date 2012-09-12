#ifndef BaseNonTerminal_H
#define BaseNonTerminal_H

#include "Symbol.h"
#include "ExprILCode.h"
#include "ExprWorkSpace.h"
#include "FuncParamsInfo.h"

//---------------------------------------------------------------------
// class declaration
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

class MatrixRowsNT;

class MatrixColsNT;

class MatrixColNT;

class FunctionNT;

//---------------------------------------------------------------------
// BaseNonTerminal - class
//---------------------------------------------------------------------
class BaseNonTerminal
{
private :
    ResultTypeEnum _ResultType;
public :
    // Static Methods
    static BaseNonTerminal* NewStartNT();
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
    virtual ResultTypeEnum GetResultType(void);
    ExprILCode* FindExprILCode(const FuncParamsInfo &funcInfo);
    ExprILCode* FindExprILCode(WordTypeEnum operWordType, ResultTypeEnum lParamType, ResultTypeEnum rParamType);
};

#endif