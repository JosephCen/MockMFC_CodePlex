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
    ExprILRunState *_pILRunState;
public :
    // Constructor
    ExprILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const = 0;
    virtual bool RunCode() = 0;
protected :
    ExprILRunState* GetILRunState();
    VariableSet* GetVariableSet();
    VariableStack* GetVariableStack();
};

inline ExprILRunState* ExprILCode::GetILRunState()
{
    _ASSERT(NULL != _pILRunState);

    return _pILRunState;
}

inline VariableSet* ExprILCode::GetVariableSet()
{
    _ASSERT(NULL != _pILRunState);

    return _pILRunState->GetVariableSet();
}

inline VariableStack* ExprILCode::GetVariableStack()
{
    _ASSERT(NULL != _pILRunState);

    return _pILRunState->GetVariableStack();
}

//---------------------------------------------------------------------
// PushIntegerILCode - class
//---------------------------------------------------------------------
class PushIntegerILCode : public ExprILCode
{
private :
    int _IntValue;
public :
    // Constructor
    explicit PushIntegerILCode(int intValue);
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual bool RunCode();
};

//---------------------------------------------------------------------
// PushRealValILCode - class
//---------------------------------------------------------------------
class PushRealValILCode : public ExprILCode
{
private :
    Matrix::RealVal_t _RealValue;
public :
    // Constructor
    explicit PushRealValILCode(Matrix::RealVal_t realValue);
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual bool RunCode();
};

//---------------------------------------------------------------------
// RealValBinaryOperILCode - class
//---------------------------------------------------------------------
class RealValBinaryOperILCode : public ExprILCode
{
private :
public :
    // Constructor
    RealValBinaryOperILCode();
    // Methods
    virtual bool RunCode();
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR) = 0;
};

//---------------------------------------------------------------------
// RealValPlusILCode - class
//---------------------------------------------------------------------
class RealValPlusILCode : public RealValBinaryOperILCode
{
private :
public :
    // Constructor
    RealValPlusILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR);
};

//---------------------------------------------------------------------
// RealValMinusILCode - class
//---------------------------------------------------------------------
class RealValMinusILCode : public RealValBinaryOperILCode
{
private :
public :
    // Constructor
    RealValMinusILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR);
};

//---------------------------------------------------------------------
// RealValMultiplyILCode - class
//---------------------------------------------------------------------
class RealValMultiplyILCode : public RealValBinaryOperILCode
{
private :
public :
    // Constructor
    RealValMultiplyILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR);
};

//---------------------------------------------------------------------
// RealValDivideILCode - class
//---------------------------------------------------------------------
class RealValDivideILCode : public RealValBinaryOperILCode
{
private :
public :
    // Constructor
    RealValDivideILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR);
};

//---------------------------------------------------------------------
// CtorMatrixILCode - class
//---------------------------------------------------------------------
class CtorMatrixILCode : public ExprILCode
{
private :
    Matrix::RowCol_t _RowColPair;
public :
    // Constructor
    CtorMatrixILCode(Matrix::Row_Col_t rows, Matrix::Row_Col_t cols);
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual bool RunCode();
};

//---------------------------------------------------------------------
// MatrixPlusILCode - class
//---------------------------------------------------------------------
class MatrixPlusILCode : public ExprILCode
{
private :
public :
    // Constructor
    MatrixPlusILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual bool RunCode();
};

#endif