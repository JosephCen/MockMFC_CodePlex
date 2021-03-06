#include "stdafx.h"
#include "Symbol.h"
#include "VariableSet.h"
#include <string>
#include <crtdbg.h>
using namespace std;

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
const int IntVariable::TypeId = RT_Integer;

IntVariable::IntVariable(int value) :
Variable(TypeId), _Value(value)
{ }

IntVariable::~IntVariable()
{ }

IMPLEMENT_VARDUP(IntVariable)

//---------------------------------------------------------------------
// Class member - RealVariable
//---------------------------------------------------------------------
const int RealVariable::TypeId = RT_RealVal;

RealVariable::RealVariable(Matrix::RealVal_t realVal) :
Variable(TypeId), _RealVal(realVal)
{ }

RealVariable::~RealVariable()
{ }

IMPLEMENT_VARDUP(RealVariable)

//---------------------------------------------------------------------
// Class member - StrVariable
//---------------------------------------------------------------------
const int StrVariable::TypeId = RT_String;

StrVariable::StrVariable(const string &strVal) :
Variable(TypeId), _StrVal(strVal)
{ }

StrVariable::StrVariable(const string *pStrVal) :
Variable(TypeId), _StrVal(*pStrVal)
{ }

StrVariable::~StrVariable()
{ }

IMPLEMENT_VARDUP(StrVariable)

//---------------------------------------------------------------------
// Class member - MatrixVariable
//---------------------------------------------------------------------
const int MatrixVariable::TypeId = RT_Matrix;

MatrixVariable::MatrixVariable() :
Variable(TypeId), _MatrixVal()
{ }

MatrixVariable::MatrixVariable(const Matrix &matrixVal) :
Variable(TypeId), _MatrixVal(matrixVal)
{ }

MatrixVariable::~MatrixVariable()
{ }

IMPLEMENT_VARDUP(MatrixVariable)

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
	_VariableMap.clear();
}

Variable_sp VariableSet::SearchVar(const std::string &varName)
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
        return (Variable_sp());
}

void VariableSet::InsertVar(const std::string &varName, Variable_sp spVariable)
{
    _ASSERT((bool)spVariable);

    _VariableMap[varName] = spVariable;
}

void VariableSet::InsertVar(const char *pCStrVarName, Variable_sp spVariable)
{
    _ASSERT(NULL != pCStrVarName);
    _ASSERT((bool)spVariable);

    InsertVar(string(pCStrVarName), spVariable);
}

void VariableSet::RemoveVar(const std::string &varName)
{
    VarMapIter_t delIter = _VariableMap.find(varName);

    if (delIter != _VariableMap.end())
        _VariableMap.erase(delIter);
}

//---------------------------------------------------------------------
// Class member - VariableStack
//---------------------------------------------------------------------
VariableStack::VariableStack()
{ }

VariableStack::~VariableStack()
{
    for (VarVecIter_t iter = _VariableVec.begin(); iter != _VariableVec.end(); ++iter)
        delete *iter;
}

void VariableStack::PushVar(Variable *pVariable)
{
    _ASSERT(NULL != pVariable);

    _VariableVec.push_back(pVariable);
}

int VariableStack::PopVar(Variable **ppVariable)
{
    _ASSERT(0 != _VariableVec.size());

    *ppVariable = _VariableVec.back();
    _VariableVec.pop_back();

    return (*ppVariable)->GetTypeId();
}

void VariableStack::RemoveTopVar()
{
    _ASSERT(0 != _VariableVec.size());

    delete _VariableVec.back();
    _VariableVec.pop_back();
}

void VariableStack::RemoveTopVar(int topCount)
{
    _ASSERT(topCount >= 0);

    while (--topCount >= 0) {
        delete _VariableVec.back();
        _VariableVec.pop_back();
    }
}