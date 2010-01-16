#include "stdafx.h"
#include "VariableSet.h"
#include <string>
using std::string;

//---------------------------------------------------------------------
// Class member - Variable
//---------------------------------------------------------------------
Variable::Variable(int typeId) :
_TypeId(typeId)
{ }

Variable::~Variable()
{ }

//---------------------------------------------------------------------
// Class member - IntVariable
//---------------------------------------------------------------------
const int IntVariable::TypeId = 1;

IntVariable::IntVariable(int value) :
Variable(TypeId), _Value(value)
{ }

IntVariable::~IntVariable()
{ }

//---------------------------------------------------------------------
// Class member - RealVariable
//---------------------------------------------------------------------
const int RealVariable::TypeId = 2;

RealVariable::RealVariable(Matrix::RealVal_t realVal) :
Variable(TypeId), _RealVal(realVal)
{ }

RealVariable::~RealVariable()
{ }

//---------------------------------------------------------------------
// Class member - StrVariable
//---------------------------------------------------------------------
const int StrVariable::TypeId = 3;

StrVariable::StrVariable(const string &strVal) :
Variable(TypeId), _StrVal(strVal)
{ }

StrVariable::StrVariable(const string *pStrVal) :
Variable(TypeId), _StrVal(*pStrVal)
{ }

StrVariable::~StrVariable()
{ }

//---------------------------------------------------------------------
// Class member - MatrixVariable
//---------------------------------------------------------------------
const int MatrixVariable::TypeId = 4;

MatrixVariable::MatrixVariable(const Matrix &matrixVal) :
Variable(TypeId), _MatrixVal(matrixVal)
{ }

MatrixVariable::~MatrixVariable()
{ }

//---------------------------------------------------------------------
// Class member - VariableSet 
//---------------------------------------------------------------------
VariableSet::VariableSet() :
_pParentSet(NULL), _OpenToSubSet(true)
{ }

VariableSet::VariableSet(VariableSet *pParentSet, bool openToSubnetSet) :
_pParentSet(pParentSet), _OpenToSubSet(openToSubnetSet)
{ }

VariableSet::~VariableSet()
{
    _pParentSet = NULL;

    for (VarMapIter_t iter = _VariableMap.begin(); iter != _VariableMap.end(); ++iter) {
        delete iter->second;
        iter->second = NULL;
    }
}

Variable* VariableSet::SearchVar(const std::string &varName)
{
    VariableSet *pCurSet = this;
    VarMapIter_t curIter = pCurSet->_VariableMap.find(varName);

    while (NULL != pCurSet && curIter == pCurSet->_VariableMap.end()) {
        do {
            pCurSet = pCurSet->_pParentSet;
        } while (NULL != pCurSet && !pCurSet->_OpenToSubSet);
        if (NULL != pCurSet)
            curIter = pCurSet->_VariableMap.find(varName);
    }

    if (NULL != pCurSet && curIter != pCurSet->_VariableMap.end())
        return curIter->second;
    else
        return NULL;
}

void VariableSet::InsertVar(const std::string &varName, Variable *pVariable)
{
    VarMapIter_t foundIter = _VariableMap.find(varName);

    if (foundIter != _VariableMap.end())
        delete foundIter->second;
    foundIter->second = pVariable;
}

void VariableSet::RemoveVar(const std::string &varName)
{
    VarMapIter_t delIter = _VariableMap.find(varName);

    if (delIter != _VariableMap.end())
    {
        delete delIter->second;
        _VariableMap.erase(delIter);
    }
}
