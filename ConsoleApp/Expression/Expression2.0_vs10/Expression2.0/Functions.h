#ifndef Functions_H
#define Functions_H

#include "BaseFunction.h"
#include "Matrix.h"


//---------------------------------------------------------------------
// EMatrixFunc - class
//---------------------------------------------------------------------
class EMatrixFunc : public BaseFunction
{
    DECLARE_FUNCINFO(EMatrixFunc)
public :
    // Constructor
    EMatrixFunc(void);
    // Methods
    virtual ResultTypeEnum GetReturnType(void) const;
    virtual FuncParamsInfo GetFuncInfo(void) const;
    virtual Variable* Call(std::vector<Variable*> *pVarVec);
};

//---------------------------------------------------------------------
// InvMatrixFunc - class
//---------------------------------------------------------------------
class InvMatrixFunc : public BaseFunction
{
    DECLARE_FUNCINFO(InvMatrixFunc)
public :
    // Constructor
    InvMatrixFunc(void);
    // Methods
    virtual ResultTypeEnum GetReturnType(void) const;
    virtual FuncParamsInfo GetFuncInfo(void) const;
    virtual Variable* Call(std::vector<Variable*> *pVarVec);
};

//---------------------------------------------------------------------
// DetMatrixFunc - class
//---------------------------------------------------------------------
class DetMatrixFunc : public BaseFunction
{
    DECLARE_FUNCINFO(DetMatrixFunc)
private :
    typedef Matrix::RealVal_t RealVal_t;
    typedef Matrix::Row_Col_t Row_Col_t;
    typedef
        struct RealValCell
        {
            RealVal_t RealVal;
            bool bSet;
        } RealValCell_t;
    typedef std::vector<RealValCell_t> CacheVec_t;
public :
    // Constructor
    DetMatrixFunc(void);
    // Methods
    virtual ResultTypeEnum GetReturnType(void) const;
    virtual FuncParamsInfo GetFuncInfo(void) const;
    virtual Variable* Call(std::vector<Variable*> *pVarVec);
private :
    Row_Col_t TransCol(Row_Col_t colMask, Row_Col_t col) const;
    RealVal_t ComputeDet(const Matrix &matRef) const;
    RealVal_t ComputerDet_Help(const Matrix &matRef, CacheVec_t &cacheVecRef, 
                               Row_Col_t rowCount, Row_Col_t colMask) const;
};

#endif