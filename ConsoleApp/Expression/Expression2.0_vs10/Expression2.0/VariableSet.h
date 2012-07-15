#ifndef VariableSet_H
#define VariableSet_H

#include <string>
#include <map>
#include <memory>
#include <vector>
#include "Matrix.h"

//---------------------------------------------------------------------
// Variable - class
//---------------------------------------------------------------------
class Variable
{
private :
    int _TypeId;
protected :
    // Constructor
    explicit Variable(int typeId);
public :
    // Constructor
    virtual ~Variable();
    // Methods
    virtual Variable* Duplicate() const = 0;
    int GetTypeId() const;
};

typedef std::tr1::shared_ptr<Variable> Variable_sp;

inline int Variable::GetTypeId() const
{
    return _TypeId;
}

//---------------------------------------------------------------------
// IntVariable - class
//---------------------------------------------------------------------
class IntVariable : public Variable
{
private :
    int _Value;
public :
    // Fields
    static const int TypeId;
    // Constructor
    explicit IntVariable(int value);
    virtual ~IntVariable();
    // Methods
    virtual Variable* Duplicate() const;
    int GetValue() const;
    int& GetValueRef();
};

inline int IntVariable::GetValue() const
{
    return _Value;
}

inline int& IntVariable::GetValueRef()
{
    return _Value;
}

//---------------------------------------------------------------------
// RealVariable - class
//---------------------------------------------------------------------
class RealVariable : public Variable
{
private :
    Matrix::RealVal_t _RealVal;
public :
    // Fields
    static const int TypeId;
    // Constructor
    explicit RealVariable(Matrix::RealVal_t realVal);
    virtual ~RealVariable();
    // Methods
    virtual Variable* Duplicate() const;
    Matrix::RealVal_t GetValue() const;
    Matrix::RealVal_t& GetValueRef();
};

inline Matrix::RealVal_t RealVariable::GetValue() const
{
    return _RealVal;
}

inline Matrix::RealVal_t& RealVariable::GetValueRef()
{
    return _RealVal;
}

//---------------------------------------------------------------------
// StrVariable - class
//---------------------------------------------------------------------
class StrVariable : public Variable
{
private :
    std::string _StrVal;
public :
    // Fields
    static const int TypeId;
    // Constructor
    explicit StrVariable(const std::string &strVal);
    explicit StrVariable(const std::string *pStrVal);
    virtual ~StrVariable();
    // Methods
    virtual Variable* Duplicate() const;
    std::string GetValue() const;
    std::string& GetValueRef();
    const std::string& GetValueRef() const;
};

inline std::string StrVariable::GetValue() const
{
    return _StrVal;
}

inline std::string& StrVariable::GetValueRef()
{
    return _StrVal;
}

inline const std::string& StrVariable::GetValueRef() const
{
    return _StrVal;
}

//---------------------------------------------------------------------
// MatrixVariable - class
//---------------------------------------------------------------------
class MatrixVariable : public Variable
{
private :
    Matrix _MatrixVal;
public :
    // Fields
    static const int TypeId;
    // Constructor
    MatrixVariable();
    explicit MatrixVariable(const Matrix &matrixVal);
    virtual ~MatrixVariable();
    // Methods
    virtual Variable* Duplicate() const;
    Matrix GetValue() const;
    Matrix& GetValueRef();
    const Matrix& GetValueRef() const;
};

inline Matrix MatrixVariable::GetValue() const
{
    return _MatrixVal;
}

inline Matrix& MatrixVariable::GetValueRef()
{
    return _MatrixVal;
}

inline const Matrix& MatrixVariable::GetValueRef() const
{
    return _MatrixVal;
}

//---------------------------------------------------------------------
// VariableSet - class
//---------------------------------------------------------------------
class VariableSet
{
private :
    typedef std::map<std::string, Variable_sp> VariableMap_t;
    typedef VariableMap_t::iterator VarMapIter_t;
    VariableMap_t _VariableMap;
    VariableSet *_pParentSet;
    bool _OpenToSubSet;
public :
    // Constructor
    VariableSet();
    VariableSet(VariableSet *pParentSet, bool openToSubnetSet);
    ~VariableSet();
    // Methods
    Variable_sp SearchVar(const std::string &varName);
    void InsertVar(const std::string &varName, Variable_sp spVariable);
    void InsertVar(const char *pCStrVarName, Variable_sp spVariable);
    void RemoveVar(const std::string &varName);
private :
    // Do not allow to copy constructor
    VariableSet(const VariableSet&);
};

//---------------------------------------------------------------------
// VariableStack - class
//---------------------------------------------------------------------
class VariableStack
{
private :
    typedef std::vector<Variable*> VariableVec_t;
    typedef VariableVec_t::iterator VarVecIter_t;
    VariableVec_t _VariableVec;
public :
    // Constructor
    VariableStack();
    ~VariableStack();
    // Methods
    int TopVar(Variable **ppVariable);
    int TopVar(Variable **ppVariable, int topIdx);
    Variable* DupTopVar() const;
    int Count() const;
    void PushVar(Variable *pVariable);
    int PopVar(Variable **ppVariable);
    void RemoveTopVar();
    void RemoveTopVar(int topCount);
private :
    // Do not allow to copy constructor
    VariableStack(const VariableStack&);
};

inline int VariableStack::TopVar(Variable **ppVariable)
{
    *ppVariable = _VariableVec.back();

    return (*ppVariable)->GetTypeId();
}

inline int VariableStack::TopVar(Variable **ppVariable, int topIdx)
{
    *ppVariable = _VariableVec[_VariableVec.size() - 1 - topIdx];

    return (*ppVariable)->GetTypeId();
}

inline Variable* VariableStack::DupTopVar() const
{
    return (_VariableVec.back()->Duplicate());
}

inline int VariableStack::Count() const
{
    return static_cast<int>(_VariableVec.size());
}

#endif