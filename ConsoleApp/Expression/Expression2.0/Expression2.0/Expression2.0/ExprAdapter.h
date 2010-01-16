#ifndef ExprAdapter_H
#define ExprAdapter_H

#include <string>
#include "ExprRunTime.h"
#include "ExprException.h"

class ExprContext
{
private :
    ExprException *_pExprEx;
public :
    // Constructor
    ExprContext(): _pExprEx(NULL) { }
    // Methods
    void SetError(const ExprException &exprExRef);
    void SetError(const char *errorCh, int chIdx = -1);
    const ExprException& GetException() const { _ASSERT(NULL != _pExprEx); return *_pExprEx; }
    // Override Operators
    operator bool() const { return (NULL == _pExprEx); }
    // Destructor
    ~ExprContext();
};

class ExprAdapter
{
private :
    bool _IsParsed;
    ExprContext _CurContext;
    ExprILSegment _ILSegment;
public :
    // Constructor
    ExprAdapter(void): _IsParsed(false), _CurContext(), _ILSegment() { }
    // Methods
    bool Parse(const std::string &exprStr);
    bool ParsedSucceed() const { return (bool)_CurContext; }
    const ExprException& GetException() const { return _CurContext.GetException(); }
    const ExprILSegment& GetILSegment() const { return _ILSegment; }
    // Destructor
    ~ExprAdapter();
};

#else

class ExprContext;
class ExprAdapter;

#endif