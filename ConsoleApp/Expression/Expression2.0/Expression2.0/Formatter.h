#ifndef Formatter_H
#define Formatter_H

#include "VariableSet.h"
#include <string>


//---------------------------------------------------------------------
// Formatter - class
//---------------------------------------------------------------------
class Formatter
{
private :
public :
    // Constructor
    Formatter();
    // Methods
    void FormatVariable(const Variable *pVar, std::string *pStr) const;
private :
    // Methods
    void FmtIntVariable(const IntVariable *pIntVar, std::ostream *pOStream) const;
    void FmtRealVariable(const RealVariable *pRealVar, std::ostream *pOStream) const;
    void FmtMatrixVariable(const MatrixVariable *pMatrixVar, std::ostream *pOStream) const;
    void FmtStrVariable(const StrVariable *pStrVar, std::ostream *pOStream) const;
    // Constructor (Do not allow copy construct)
    Formatter(const Formatter&);
};

#endif