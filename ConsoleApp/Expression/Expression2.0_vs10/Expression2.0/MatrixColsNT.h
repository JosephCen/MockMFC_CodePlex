#ifndef MatrixColsNT_H
#define MatrixColsNT_H

#include "BaseNonTerminal.h"
#include <list>

//---------------------------------------------------------------------
// MatrixColsNT - class
//---------------------------------------------------------------------
class MatrixColsNT : public BaseNonTerminal
{
private :
    typedef std::list<MatrixColNT*> ColList_t;
    ColList_t _ColList;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    MatrixColsNT(void);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    Matrix::Row_Col_t Cols() const { return _ColList.size(); }
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    virtual ~MatrixColsNT();
private :
	// Constructor (Do not allow copy construct)
    MatrixColsNT(const MatrixColsNT&);
	// Assignment operator (Do not allow assignment operator)
	MatrixColsNT& operator=(const MatrixColsNT&);
};

#endif