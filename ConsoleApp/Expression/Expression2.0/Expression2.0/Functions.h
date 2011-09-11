#ifndef Functions_H
#define Functions_H

#include "BaseFunction.h"
#include "Matrix.h"

class EMatrixFunc : public BaseFunction
{
    DECLARE_FUNCINFO(EMatrixFunc)
public :
    // Constructor
    EMatrixFunc(void);
    // Methods
    virtual ResultTypeEnum GetReturnType(void) const;
    virtual Variable* Call(std::vector<Variable*> *pVarVec);
};

#endif