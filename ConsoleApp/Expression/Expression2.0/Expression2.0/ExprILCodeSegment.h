#ifndef ExprILCodeSegment_H
#define ExprILCodeSegment_H

#include <vector>
#include <string>
#include <sstream>
#include "ExprILCode.h"

//---------------------------------------------------------------------
// ExprILCodeSegment - class
//---------------------------------------------------------------------
class ExprILCodeSegment
{
private :
    typedef std::vector<ExprILCode*> ILCodeVec_t;
    typedef std::vector<ExprILCode*>::iterator ILCodeIter_t;
    typedef std::vector<ExprILCode*>::const_iterator ILCodeCIter_t;
    ILCodeVec_t _ILCodeVec;
public :
    // Constructor
    ExprILCodeSegment();
    ~ExprILCodeSegment();
    // Methods
    void Append(ExprILCode *pILCode);
    bool Run(VariableSet *pVariableSet, Variable **ppVariable);
    int Length() const;
    virtual void ToString(std::ostringstream *pOStrStream) const;
    std::string ToString() const;
private :
    // Constructor (Do not allow copy construct)
    ExprILCodeSegment(const ExprILCodeSegment &other);
};

inline void ExprILCodeSegment::Append(ExprILCode *pILCode)
{
    _ILCodeVec.push_back(pILCode);
}

inline int ExprILCodeSegment::Length() const
{
    return static_cast<int>(_ILCodeVec.size());
}

#endif