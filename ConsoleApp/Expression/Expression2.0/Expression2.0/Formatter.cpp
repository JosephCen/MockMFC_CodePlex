#include "stdafx.h"
#include "Formatter.h"
#include "VariableSet.h"
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <crtdbg.h>


//---------------------------------------------------------------------
// Class member - VarFormatter
//---------------------------------------------------------------------
Formatter::Formatter()
{ }

void Formatter::FormatVariable(const Variable *pVar, std::string *pStr) const
{
    _ASSERT(NULL != pVar);
    _ASSERT(NULL != pStr);

    ostringstream oStrStream;

    if (IntVariable::TypeId == pVar->GetTypeId())
        FmtIntVariable((IntVariable*)pVar, &oStrStream);
    else if (RealVariable::TypeId == pVar->GetTypeId())
        FmtRealVariable((RealVariable*)pVar, &oStrStream);
    else if (StrVariable::TypeId == pVar->GetTypeId())
        FmtStrVariable((StrVariable*)pVar, &oStrStream);
    else if (MatrixVariable::TypeId == pVar->GetTypeId())
        FmtMatrixVariable((MatrixVariable*)pVar, &oStrStream);
    else
        _ASSERT(0); // A unknown variable.

    pStr->assign(oStrStream.str());
}

void Formatter::FmtIntVariable(const IntVariable *pIntVar, std::ostream *pOStream) const
{
    *pOStream << pIntVar->GetValue();
}

void Formatter::FmtRealVariable(const RealVariable *pRealVar, std::ostream *pOStream) const
{
    *pOStream << pRealVar->GetValue();
}

void Formatter::FmtStrVariable(const StrVariable *pStrVar, std::ostream *pOStream) const
{
    *pOStream << pStrVar->GetValueRef();
}

void Formatter::FmtMatrixVariable(const MatrixVariable *pMatrixVar, std::ostream *pOStream) const
{
    if (pMatrixVar->GetValueRef().IsSingleValue())
        *pOStream << pMatrixVar->GetValueRef().At(0, 0);
    else
        *pOStream << pMatrixVar->GetValueRef().ToString();
}