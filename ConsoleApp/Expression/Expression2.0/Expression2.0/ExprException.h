#ifndef ExprException_H
#define ExprException_H

#include <string>
#include <exception>
#include <sstream>

class ExprException : public std::exception
{
private :
    int _ChIndex;
public :
    // Constructor
    ExprException(const char *msgCh, int chIdx = -1): std::exception(msgCh), _ChIndex(chIdx) { }
    // Methods
    const char* Message() const { return what(); }
    int ErrorChIdx() const { return (int)_ChIndex; }
    void ToString(std::ostringstream *pOStrStream) const;
    std::string ToString() const;
};

class ExprExHolder
{
private :
    ExprException *_pExprEx;
public :
    // Constructor
    ExprExHolder();
    virtual ~ExprExHolder();
    // Methods
    void SetError(const ExprException &exprExRef);
    void SetError(const char *errorCh, int chIdx = -1);
    const ExprException& GetException() const;
    void GetExceptionStr(std::string *pExStr) const;
    // Override Operators
    operator bool() const;
};

#endif