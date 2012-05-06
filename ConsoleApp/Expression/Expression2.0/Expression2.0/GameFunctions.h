#ifndef GameFunctions_H
#define GameFunctions_H

#include <vector>
#include "BaseFunction.h"
#include "Matrix.h"

//---------------------------------------------------------------------
// Point24Func - class
//---------------------------------------------------------------------
class Point24Func : public BaseFunction
{
    DECLARE_FUNCINFO(Point24Func)
public :
    // Constructor
    Point24Func(void);
    // Methods
    virtual ResultTypeEnum GetReturnType(void) const;
    virtual FuncParamsInfo GetFuncInfo(void) const;
    virtual Variable* Call(std::vector<Variable*> *pVarVec);
private :
    void Calculate(const Matrix &matRef, Matrix::RealVal_t finalVal, std::string &resultStrRef);
};

#endif