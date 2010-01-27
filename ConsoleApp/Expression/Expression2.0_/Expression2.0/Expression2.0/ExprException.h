#ifndef ExprException_H
#define ExprException_H

#include <string>
#include <exception>

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
};

#endif