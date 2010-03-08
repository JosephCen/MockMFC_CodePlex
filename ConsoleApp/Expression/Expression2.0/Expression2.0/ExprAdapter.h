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

#endif