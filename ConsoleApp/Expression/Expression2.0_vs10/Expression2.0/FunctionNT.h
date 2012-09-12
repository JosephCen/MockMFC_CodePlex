#ifndef FunctionNT_H
#define FunctionNT_H

#include "BaseNonTerminal.h"

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
    const ExprILCode *_pExprILCode;
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