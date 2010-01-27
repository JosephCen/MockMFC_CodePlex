#ifndef ExprILCode_H
#define ExprILCode_H

#include "VariableSet.h"
#include "ExprRunTime.h"

//---------------------------------------------------------------------
// ExprILRunState - class
//---------------------------------------------------------------------
class ExprILRunState
{
private :
    bool  _IsNewVarSet;
    VariableSet *_pVarSet;
    VariableStack _VarStack;
public :
    // Constructor
    ExprILRunState(VariableSet *pVarSet, bool isOwnVarSet);
    ~ExprILRunState();
    // Methods
    VariableSet* GetVariableSet();
    VariableStack* GetVariableStack();
private :
    // Do not allow to copy constructor
    ExprILRunState(const ExprILRunState&);
};

inline VariableSet* ExprILRunState::GetVariableSet()
{
    return _pVarSet;
}

inline VariableStack* ExprILRunState::GetVariableStack()
{
    return &_VarStack;
}

//---------------------------------------------------------------------
// ExprILCode - class
//---------------------------------------------------------------------
class ExprILCode
{
private :
    ExprILCodeEnum _Code;
public :
    // Constructor
    ExprILCode();
    // Methods
    
};

#endif