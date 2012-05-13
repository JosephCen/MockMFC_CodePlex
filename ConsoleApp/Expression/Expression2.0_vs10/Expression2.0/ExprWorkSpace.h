#ifndef ExprWorkSpace_H
#define ExprWorkSpace_H

#include "ClsPreDeclaration.h"
#include "VariableSet.h"
#include "WordParser.h"
#include "ExprILCode.h"
#include "ExprException.h"
#include <vector>
#include <string>
#include <sstream>

//---------------------------------------------------------------------
// ExprILCodeSegment - class
//---------------------------------------------------------------------
class ExprILCodeSegment : public ExprErrHolder
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
    pILCode->SetOwner(this);
    _ILCodeVec.push_back(pILCode);
}

inline int ExprILCodeSegment::Length() const
{
    return static_cast<int>(_ILCodeVec.size());
}

//---------------------------------------------------------------------
// ExprWorkSpace - class
//---------------------------------------------------------------------
class ExprWorkSpace : public ExprErrHolder
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
    friend class WordParser;
};

#endif