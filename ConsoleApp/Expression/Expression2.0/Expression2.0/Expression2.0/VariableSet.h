#ifndef VariableSet_H
#define VariableSet_H

#include <string>
#include <map>
#include <vector>
#include "Matrix.h"

//---------------------------------------------------------------------
// Variable - class
//---------------------------------------------------------------------
class Variable
{
private :
    int _TypeId;
public :
    // Constructor
    explicit Variable(int typeId);
    virtual ~Variable();
    // Methods
    int GetTypeId() const;
};

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
    int GetValue() const;
};

inline int IntVariable::GetValue() const
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
    Matrix::RealVal_t GetValue() const;
};

inline Matrix::RealVal_t RealVariable::GetValue() const
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
    std::string GetValue() const;
    std::string& GetValueRef();
};

inline std::string StrVariable::GetValue() const
{
    return _StrVal;
}

inline std::string& StrVariable::GetValueRef()
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
    explicit MatrixVariable(const Matrix &matrixVal);
    virtual ~MatrixVariable();
    // Methods
    Matrix GetValue() const;
    Matrix& GetValueRef();
};

inline Matrix MatrixVariable::GetValue() const
{
    return _MatrixVal;
}

inline Matrix& MatrixVariable::GetValueRef()
{
    return _MatrixVal;
}

//---------------------------------------------------------------------
// VariableSet - class
//---------------------------------------------------------------------
class VariableSet
{
private :
    typedef std::map<std::string, Variable*> VariableMap_t;
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
    Variable* SearchVar(const std::string &varName);
    void InsertVar(const std::string &varName, Variable *pVariable);
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
    Variable* TopVar();
    int Count() const;
    void PushVar(Variable *pVariable);
    void PopVar(Variable **ppVariable);
    void RemoveTopVar();
private :
    // Do not allow to copy constructor
    VariableStack(const VariableStack&);
};

inline Variable* VariableStack::TopVar()
{
    return _VariableVec.back();
}

inline int VariableStack::Count() const
{
    return static_cast<int>(_VariableVec.size());
}

#endif