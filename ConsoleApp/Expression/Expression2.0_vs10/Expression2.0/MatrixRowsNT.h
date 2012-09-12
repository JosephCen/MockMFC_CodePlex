#ifndef MatrixRowsNT_H
#define MatrixRowsNT_H

#include "BaseNonTerminal.h"
#include <list>

//---------------------------------------------------------------------
// MatrixRowsNT - class
//---------------------------------------------------------------------
class MatrixRowsNT : public BaseNonTerminal
{
private :
    typedef std::list<MatrixColsNT*> RowList_t;
    RowList_t _RowList;
public :
    // Static Methods
    static bool IsInFirstSet(WordTypeEnum wordType);
    // Constructor
    MatrixRowsNT(void);
    // Methods
    virtual bool Parse(ExprContext &exprContextRef, WordFwCursor &wordCursorRef);
    Matrix::Row_Col_t Rows() const { return _RowList.size(); }
    Matrix::Row_Col_t Cols() const;
    virtual ExprILCodeSegment& AppendILSegment(ExprILCodeSegment &ilSegment);
    // Destructor
    virtual ~MatrixRowsNT();
private :
	// Constructor (Do not allow copy construct)
    MatrixRowsNT(const MatrixRowsNT&);
	// Assignment operator (Do not allow assignment operator)
	MatrixRowsNT& operator=(const MatrixRowsNT&);
};

#endif