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
    Variable_sp Run();
    int Length() const;
    virtual void ToString(std::ostream *pOStream) const;
    std::string ToString() const;
private :
    // Constructor (Do not allow copy construct)
    ExprILCodeSegment(const ExprILCodeSegment&);
	// Assignment operator (Do not allow assignment operator)
	ExprILCodeSegment& operator=(const ExprILCodeSegment&);
};

typedef std::tr1::shared_ptr<ExprILCodeSegment> ExprILCodeSegment_sp;

inline void ExprILCodeSegment::Append(ExprILCode *pILCode)
{
    _ASSERT(nullptr != pILCode);

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
    typedef std::vector<ExprILCodeSegment_sp> ILCodeSegmentVec_t;
    typedef std::vector<ExprILCodeSegment_sp>::iterator ILCodeSegmentIter_t;

    VariableSet _GlobalVarSet;
    WordParser _WordParser;
    ILCodeSegmentVec_t _ILCodeSegmentSet;
    bool _IsDesructing;
public :
    // Constructor
    ExprWorkSpace();
    ~ExprWorkSpace();
    // Methods
    ExprILCodeSegment_sp ParseILCodeSegment(const std::string &codeStr);
    Variable_sp RunILCodeSegment(ExprILCodeSegment_sp spILSegment);
private :
	void RemoveILCodeSegment(ExprILCodeSegment *pILSegment);
    // Constructor (Do not allow copy construct)
    ExprWorkSpace(const ExprWorkSpace&);
    // Friend declaration
    friend class ExprILCodeSegment;
    friend class WordParser;
};

#endif