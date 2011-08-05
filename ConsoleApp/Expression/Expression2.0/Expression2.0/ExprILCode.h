#ifndef ExprILCode_H
#define ExprILCode_H

#include <string>
#include <sstream>
#include "VariableSet.h"
#include "ExprRunTime.h"
#include "ExprException.h"

//---------------------------------------------------------------------
// ExprILRunState - class
//---------------------------------------------------------------------
class ExprILRunState : public ExprErrHolder
{
private :
    bool  _IsNewVarSet;
    VariableSet *_pVarSet;
    VariableStack _VarStack;
public :
    // Constructor
    ExprILRunState(VariableSet *pVarSet, bool isNewVarSet);
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
public :
    // Constructor
    ExprILCode();
    virtual ~ExprILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const = 0;
    virtual bool RunCode(ExprILRunState *pILRunState) = 0;
    virtual void ToString(std::ostream *pOStream) const = 0;
    std::string ToString() const;
protected :
    static VariableStack* GetVariableStack(ExprILRunState *pILRunState);
};

inline VariableStack* ExprILCode::GetVariableStack(ExprILRunState *pILRunState)
{
    return pILRunState->GetVariableStack();
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
    virtual bool RunCode(ExprILRunState *pILRunState);
    virtual void ToString(std::ostream *pOStream) const;
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
    virtual bool RunCode(ExprILRunState *pILRunState);
    virtual void ToString(std::ostream *pOStream) const;
};

//---------------------------------------------------------------------
// PushDefValILCode - class
//---------------------------------------------------------------------
class PushDefValILCode : public ExprILCode
{
private :
    std::string _DefValName;
public :
    // Constructor
    explicit PushDefValILCode(const std::string &defValName);
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual bool RunCode(ExprILRunState *pILRunState);
    virtual void ToString(std::ostream *pOStream) const;
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
    virtual bool RunCode(ExprILRunState *pILRunState);
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState) = 0;
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
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState);
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
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState);
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
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState);
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
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(RealVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState);
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
    virtual bool RunCode(ExprILRunState *pILRunState);
    virtual void ToString(std::ostream *pOStream) const;
};

//---------------------------------------------------------------------
// MatrixBinaryOperILCode - class
//---------------------------------------------------------------------
class MatrixBinaryOperILCode : public ExprILCode
{
private :
public :
    // Constructor
    MatrixBinaryOperILCode();
    // Methods
    virtual bool RunCode(ExprILRunState *pILRunState);
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState) = 0;
};

//---------------------------------------------------------------------
// MatrixPlusILCode - class
//---------------------------------------------------------------------
class MatrixPlusILCode : public MatrixBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixPlusILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState);
};

//---------------------------------------------------------------------
// MatrixMinusILCode - class
//---------------------------------------------------------------------
class MatrixMinusILCode : public MatrixBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixMinusILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState);
};

//---------------------------------------------------------------------
// MatrixMultiplyILCode - class
//---------------------------------------------------------------------
class MatrixMultiplyILCode : public MatrixBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixMultiplyILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState);
};

//---------------------------------------------------------------------
// MatrixDotMultiplyILCode - class
//---------------------------------------------------------------------
class MatrixDotMultiplyILCode : public MatrixBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixDotMultiplyILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState);
};

//---------------------------------------------------------------------
// MatrixDivideILCode - class
//---------------------------------------------------------------------
class MatrixDivideILCode : public MatrixBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixDivideILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState);
};

//---------------------------------------------------------------------
// MatrixDotDivideILCode - class
//---------------------------------------------------------------------
class MatrixDotDivideILCode : public MatrixBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixDotDivideILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, MatrixVariable *pVariableR, ExprILRunState *pILRunState);
};

//---------------------------------------------------------------------
// MatrixValBinaryOperILCode - class
//---------------------------------------------------------------------
class MatrixValBinaryOperILCode : public ExprILCode
{
private :
public :
    // Constructor
    MatrixValBinaryOperILCode();
    // Methods
    virtual bool RunCode(ExprILRunState *pILRunState);
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState) = 0;
};

//---------------------------------------------------------------------
// MatrixValPlusILCode - class
//---------------------------------------------------------------------
class MatrixValPlusILCode : public MatrixValBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixValPlusILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState);
};

//---------------------------------------------------------------------
// MatrixValMinusILCode - class
//---------------------------------------------------------------------
class MatrixValMinusILCode : public MatrixValBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixValMinusILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState);
};

//---------------------------------------------------------------------
// MatrixValMultiplyILCode - class
//---------------------------------------------------------------------
class MatrixValMultiplyILCode : public MatrixValBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixValMultiplyILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState);
};

//---------------------------------------------------------------------
// MatrixValDivideILCode - class
//---------------------------------------------------------------------
class MatrixValDivideILCode : public MatrixValBinaryOperILCode
{
private :
public :
    // Constructor
    MatrixValDivideILCode();
    // Methods
    virtual ExprILCodeEnum GetCodeEnum() const;
    virtual void ToString(std::ostream *pOStream) const;
protected :
    // Methods
    virtual bool DoOperator(MatrixVariable *pVariableL, RealVariable *pVariableR, ExprILRunState *pILRunState);
};

#endif