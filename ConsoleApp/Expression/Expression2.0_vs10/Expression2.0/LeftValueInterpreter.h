#ifndef LeftValueInterpreter_H
#define LeftValueInterpreter_H

#include "BaseNonTerminal.h"

class LeftValueInterpreter
{
private :
    bool _IsLeftValue;
    bool _IsDefinite;
public :
    // Constructor
    LeftValueInterpreter(void);
    // Methods
    void Interpret(StartNT *pStartNT);
    void Interpret(ListNT *pListNT);
    void Interpret(CodeLineNT *pCodeLineNT);
    void Interpret(AssignExprNT *pAssignExprNT);
    void Interpret(ExprNT *pExprNT);
    void Interpret(SubExprNT *pSubExprNT);
    void Interpret(TermNT *pTermNT);
    void Interpret(SubTermNT *pSubTermNT);
    void Interpret(FactorNT *pFactorNT);
    void Interpret(MatrixNT *pMatrixNT);
    void Interpret(FunctionNT *pFunctionNT);
    bool GetIsLeftValue() const;
    bool GetIsDefinite() const;
private :
    void SetIsLeftValue(bool isLeftValue);
};

#endif