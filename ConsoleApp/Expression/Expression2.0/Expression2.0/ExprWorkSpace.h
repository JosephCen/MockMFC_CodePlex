#ifndef ExprWorkSpace_H
#define ExprWorkSpace_H

#include "VariableSet.h"
#include "WordParser.h"
#include "ExprILCodeSegment.h"
#include <vector>
#include <string>
#include <sstream>

class ExprWorkSpace
{
private :
    typedef std::vector<ExprILCodeSegment*> ILCodeSegmentVec_t;
    typedef std::vector<ExprILCodeSegment*>::iterator ILCodeSegmentIter_t;

    VariableSet _GlobalVarSet;
    WordParser _WordParser;
    ILCodeSegmentVec_t _ILCodeSegmentSet;
public :
    // Constructor
    ExprWorkSpace();
    ~ExprWorkSpace();
    // Methods
    bool ParseILCodeSegment(const std::string &codeStr, ExprILCodeSegment **ppILSegment);
    bool RunILCodeSegment(ExprILCodeSegment *pILSegment, Variable **ppVariable);
private :
    // Constructor (Do not allow copy construct)
    ExprWorkSpace(const ExprWorkSpace&);
};

#endif