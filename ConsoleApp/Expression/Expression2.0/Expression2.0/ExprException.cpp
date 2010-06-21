#include "stdafx.h"
#include "ExprException.h"
#include <crtdbg.h>
#include <string>
using std::string;
#include <exception>
#include <sstream>
using std::ostringstream;

//---------------------------------------------------------------------
// Class member - ExprException
//---------------------------------------------------------------------
void ExprException::ToString(ostringstream *pOStrStream) const
{
    _ASSERT(NULL != pOStrStream);

    if (_ChIndex >= 0)
        *pOStrStream << "At [" << _ChIndex << "]: " << Message();
    else
        *pOStrStream << Message();
}

string ExprException::ToString() const
{
    ostringstream oStrStream;

    ToString(&oStrStream);

    return (oStrStream.str());
}

//---------------------------------------------------------------------
// Class member - ExprExHolder
//---------------------------------------------------------------------
ExprExHolder::ExprExHolder() :
_pExprEx(NULL)
{ }

ExprExHolder::~ExprExHolder()
{
    if (_pExprEx) {
        delete _pExprEx;
        _pExprEx = NULL;
    }
}

void ExprExHolder::SetError(const ExprException &exprExRef)
{
    _ASSERT(NULL == _pExprEx);

    _pExprEx = new ExprException(exprExRef);
}

void ExprExHolder::SetError(const char *errorCh, int chIdx)
{
    _ASSERT(NULL == _pExprEx);

    _pExprEx = new ExprException(errorCh, chIdx);
}

const ExprException& ExprExHolder::GetException() const
{
    _ASSERT(NULL == _pExprEx);

    return *_pExprEx;
}

ExprExHolder::operator bool() const
{
    return (NULL == _pExprEx);
}

void ExprExHolder::GetExceptionStr(std::string *pExStr) const
{
    _ASSERT(NULL != pExStr);

    pExStr->clear();
    if (NULL != _pExprEx)
    {
        ostringstream oStrStream;

        _pExprEx->ToString(&oStrStream);
        pExStr->append(oStrStream.str());
    }
}
