#ifndef ExprException_H
#define ExprException_H

#include <string>
#include <exception>
#include <sstream>
#include <vector>

//---------------------------------------------------------------------
// Class Declaration
//---------------------------------------------------------------------
class ExprError;

//---------------------------------------------------------------------
// ExprException - class
//---------------------------------------------------------------------
class ExprException : public std::exception
{
private :
    int _RefCount;
    int _ChIndex;
public :
    // Constructor
    ExprException(const char *msgCh, int chIdx = -1);
    ~ExprException();
    // Methods
    const char* Message() const { return what(); }
    int ErrorChIdx() const { return (int)_ChIndex; }
    void ToString(std::ostream *pOStream) const;
    std::string ToString() const;
    // Friend Class Declaration
    friend class ExprError;
};

//---------------------------------------------------------------------
// ExprError - class
//---------------------------------------------------------------------
class ExprError
{
private :
    ExprException *_pExprEx;
public :
    // Constructor
    ExprError();
    ExprError(const ExprError &exprErrRef);
    ~ExprError();
    // Methods
    bool IsContainError() const;
    void Set(const ExprException &exprExRef);
    void Set(const char *errorCh, int chIdx);
    const ExprException& GetExprException() const;
    void ToString(std::ostream *pOStream) const;
    std::string ToString() const;
    // Override Operators
    ExprError& operator=(const ExprError &exprErrRef);
private :
    // Methods
    void ReleaseCurrentEx();
};

inline bool ExprError::IsContainError() const
{
    return (NULL != _pExprEx);
}

inline void ExprError::ToString(std::ostream *pOStream) const
{
    if (IsContainError())
        _pExprEx->ToString(pOStream);
}

//---------------------------------------------------------------------
// ExprExHolder - class
//---------------------------------------------------------------------
class ExprErrHolder
{
private :
    typedef std::vector<ExprError> ExprErrorVec_t;
    typedef std::vector<ExprError>::iterator ExprErrorIter_t;
    typedef std::vector<ExprError>::const_iterator ExprErrorCIter_t;

    ExprErrorVec_t _ExprErrVec;
public :
    // Constructor
    ExprErrHolder();
    virtual ~ExprErrHolder();
    // Methods
    void SetError(const ExprException &exprExRef);
    void SetError(const char *errorCh, int chIdx = -1);
    const ExprException& GetException(int idx) const;
    int GetErrorCount() const;
    void GetExceptionStr(std::string *pExStr) const;
    void ClearError();
    void MergeError(const ExprErrHolder &errHolderRef);
    void MergeError(const ExprErrHolder *pErrHolder);
    // Override Operators
    operator bool() const;
private :
    // Constructor (Do not allow copy construct)
    ExprErrHolder(const ExprErrHolder&);
	// Assignment operator (Do not allow assignment operator)
	ExprErrHolder& operator=(const ExprErrHolder&);
};

inline void ExprErrHolder::SetError(const ExprException &exprExRef)
{
    ExprError exprErr;

    exprErr.Set(exprExRef);
    _ExprErrVec.push_back(exprErr);
}

inline void ExprErrHolder::SetError(const char *errorCh, int chIdx)
{
    ExprError exprErr;

    exprErr.Set(errorCh, chIdx);
    _ExprErrVec.push_back(exprErr);
}

inline const ExprException& ExprErrHolder::GetException(int idx) const
{
    return (_ExprErrVec.at(static_cast<size_t>(idx)).GetExprException());
}

inline int ExprErrHolder::GetErrorCount() const
{
    return (static_cast<int>(_ExprErrVec.size()));
}

inline void ExprErrHolder::ClearError()
{
    _ExprErrVec.clear();
}

inline void ExprErrHolder::MergeError(const ExprErrHolder &errHolderRef)
{
    _ExprErrVec.insert(_ExprErrVec.end(), errHolderRef._ExprErrVec.begin(), errHolderRef._ExprErrVec.end());
}

inline void ExprErrHolder::MergeError(const ExprErrHolder *pErrHolder)
{
    _ExprErrVec.insert(_ExprErrVec.end(), pErrHolder->_ExprErrVec.begin(), pErrHolder->_ExprErrVec.end());
}

inline ExprErrHolder::operator bool() const
{
    return (_ExprErrVec.size() == 0);
}

#endif