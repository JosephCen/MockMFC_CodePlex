#ifndef ExprWorkSpace_H
#define ExprWorkSpace_H

#include "VariableSet.h"
#include "WordParser.h"
#include "ExprILCodeSegment.h"
#include <string>
#include <sstream>

class ExprWorkSpace
{
private :
    VariableSet _GlobalVarSet;
    WordParser _WordParser;
public :
    // Constructor
    ExprWorkSpace();
    // Methods
    bool ParseILCodeSegment(const std::string &codeStr, ExprILCodeSegment *pILSegment);
private :
    // Constructor (Do not allow copy construct)
    ExprWorkSpace(const ExprWorkSpace&);
};

#endif