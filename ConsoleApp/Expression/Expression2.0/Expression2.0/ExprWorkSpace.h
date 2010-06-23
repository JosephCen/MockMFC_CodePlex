#ifndef ExprWorkSpace_H
#define ExprWorkSpace_H

#include "VariableSet.h"
#include "WordParser.h"
#include "ExprILCode.h"
#include <vector>
#include <string>
#include <sstream>

//---------------------------------------------------------------------
// class declaration
//---------------------------------------------------------------------
class ExprILCodeSegment;

class ExprWorkSpace;

//---------------------------------------------------------------------
// ExprILCodeSegment - class
//---------------------------------------------------------------------
class ExprILCodeSegment
{
private :
    typedef std::vector<ExprILCode*> ILCodeVec_t;
    typedef std::vector<ExprILCode*>::iterator ILCodeIter_t;
    typedef std::vector<ExprILCode*>::const_iterator ILCodeCIter_t;
    
    ExprWorkSpace *_pWorkSpace;
    ILCodeVec_t _ILCodeVec;
public :
    // Constructor
    ExprILCodeSegment(ExprWorkSpace *pWorkSpace);
    ~ExprILCodeSegment();
    // Methods
    void Append(ExprILCode *pILCode);
    bool Run(Variable **ppVariable);
    int Length() const;
    virtual void ToString(std::ostream *pOStream) const;
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

//---------------------------------------------------------------------
// ExprWorkSpace - class
//---------------------------------------------------------------------
class ExprWorkSpace
{
private :
    typedef std::vector<ExprILCodeSegment*> ILCodeSegmentVec_t;
    typedef std::vector<ExprILCodeSegment*>::iterator ILCodeSegmentIter_t;

    VariableSet _GlobalVarSet;
    WordParser _WordParser;
    ILCodeSegmentVec_t _ILCodeSegmentSet;
    bool _IsDesructing;
public :
    // Constructor
    ExprWorkSpace();
    ~ExprWorkSpace();
    // Methods
    bool ParseILCodeSegment(const std::string &codeStr, ExprILCodeSegment **ppILSegment);
    bool RunILCodeSegment(ExprILCodeSegment *pILSegment, Variable **ppVariable);
    void RemoveILCodeSegment(ExprILCodeSegment *pILSegment);
private :
    // Constructor (Do not allow copy construct)
    ExprWorkSpace(const ExprWorkSpace&);
    // Friend declaration
    friend class ExprILCodeSegment;
};

#endif