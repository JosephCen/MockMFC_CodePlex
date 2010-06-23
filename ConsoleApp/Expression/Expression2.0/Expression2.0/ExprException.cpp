#include "stdafx.h"
#include "ExprException.h"
#include <string>
using std::string;
#include <exception>
using std::exception;
#include <sstream>
using std::ostream;
using std::ostringstream;
#include <vector>
using std::vector;
#include <crtdbg.h>

//---------------------------------------------------------------------
// Class member - ExprException
//---------------------------------------------------------------------
ExprException::ExprException(const char *msgCh, int chIdx/* = -1*/) :
exception(msgCh), _ChIndex(chIdx), _RefCount(1)
{ }

ExprException::~ExprException()
{
    _ASSERT(1 == _RefCount);
}

void ExprException::ToString(ostream *pOStream) const
{
    _ASSERT(NULL != pOStream);

    if (_ChIndex >= 0)
        *pOStream << "At [" << _ChIndex << "]: " << Message();
    else
        *pOStream << Message();
}

string ExprException::ToString() const
{
    ostringstream oStrStream;

    ToString(&oStrStream);

    return (oStrStream.str());
}

//---------------------------------------------------------------------
// Class member - ExprError
//---------------------------------------------------------------------
ExprError::ExprError() :
_pExprEx(NULL)
{ }

ExprError::ExprError(const ExprError &exprErrRef) :
_pExprEx(exprErrRef._pExprEx)
{
     if (NULL != _pExprEx)
        ++_pExprEx->_RefCount;
}

ExprError::~ExprError()
{
    if (IsContainError())
        ReleaseCurrentEx();
}

ExprError& ExprError::operator=(const ExprError &exprErrRef)
{
    if (this != &exprErrRef)
    {
        // Release original ExprException instance
        if (IsContainError())
            ReleaseCurrentEx();

        // Reference new ExprException instance
        _pExprEx = exprErrRef._pExprEx;
        if (NULL != _pExprEx)
            ++_pExprEx->_RefCount;
    }

    return *this;
}

void ExprError::Set(const ExprException &exprExRef)
{
    if (IsContainError())
        ReleaseCurrentEx();
    _pExprEx = new ExprException(exprExRef);
}

void ExprError::Set(const char *errorCh, int chIdx)
{
    _ASSERT(NULL != errorCh);

    if (IsContainError())
        ReleaseCurrentEx();
    _pExprEx = new ExprException(errorCh, chIdx);
}

const ExprException& ExprError::GetExprException() const
{
    _ASSERT(NULL != _pExprEx);

    return (*_pExprEx);
}

string ExprError::ToString() const
{
    ostringstream oStrStream;

    ToString(&oStrStream);

    return (oStrStream.str());
}

void ExprError::ReleaseCurrentEx()
{
    if (NULL != _pExprEx)
    {
        if (1 == _pExprEx->_RefCount)
            delete _pExprEx;
        else
            --_pExprEx->_RefCount;

        _pExprEx = NULL;
    }
}

//---------------------------------------------------------------------
// Class member - ExprExHolder
//---------------------------------------------------------------------
ExprErrHolder::ExprErrHolder() :
_ExprErrVec(0)
{ }

ExprErrHolder::~ExprErrHolder()
{ }

void ExprErrHolder::GetExceptionStr(std::string *pExStr) const
{
    _ASSERT(NULL != pExStr);

    ostringstream oStrStream;
    
    for (ExprErrorCIter_t iter = _ExprErrVec.begin(); _ExprErrVec.end() != iter; ++iter)
    {
        iter->ToString(&oStrStream);
        oStrStream << '\n';
    }
    // Assign error content into current string. (Remove the last '\n' char)
    pExStr->assign(oStrStream.str(), 0, oStrStream.str().length() - 1);
}
