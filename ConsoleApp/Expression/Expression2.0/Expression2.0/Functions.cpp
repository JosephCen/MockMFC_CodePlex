#include "stdafx.h"
#include "DoubleComparison.h"
#include "ExprException.h"
#include "Functions.h"
#include <crtdbg.h>
using namespace std;


//---------------------------------------------------------------------
// Class member - EMatrixFunc
//---------------------------------------------------------------------
IMPLEMENT_FUNCINFO(EMatrixFunc);

EMatrixFunc::EMatrixFunc(void)
{ }

ResultTypeEnum EMatrixFunc::GetReturnType(void) const
{
    return RT_Matrix;
}

FuncParamsInfo EMatrixFunc::GetFuncInfo(void) const
{
    return FuncParamsInfo("E", RT_RealVal);
}

Variable* EMatrixFunc::Call(std::vector<Variable*> *pVarVec)
{
    RealVariable *pRealVar = (RealVariable*)(*pVarVec)[0];
    Matrix::RealVal_t realVal = pRealVar->GetValue();
    Matrix::Row_Col_t rowCol = static_cast<Matrix::Row_Col_t>(realVal);

    if (DOUBLE_NEQ(realVal, rowCol))
        throw ExprException("The argument of function E must be an Integer.");

    return (new MatrixVariable(Matrix(rowCol, rowCol, 'E')));
}

//---------------------------------------------------------------------
// Class member - InvMatrixFunc
//---------------------------------------------------------------------
IMPLEMENT_FUNCINFO(InvMatrixFunc);

InvMatrixFunc::InvMatrixFunc(void)
{ }

ResultTypeEnum InvMatrixFunc::GetReturnType(void) const
{
    return RT_Matrix;
}

FuncParamsInfo InvMatrixFunc::GetFuncInfo(void) const
{
    return FuncParamsInfo("Inv", RT_Matrix);
}

Variable* InvMatrixFunc::Call(std::vector<Variable*> *pVarVec)
{
    MatrixVariable *pMatVar = (MatrixVariable*)(*pVarVec)[0];
    MatrixVariable *pRetMatVar = new MatrixVariable(*pMatVar);

    try {
        pRetMatVar->GetValueRef().ToInversion();
    }
    catch (...) {
        delete pRetMatVar;
        pRetMatVar = NULL;
        throw;
    }

    return pRetMatVar;
}

//---------------------------------------------------------------------
// Class member - DetMatrixFunc
//---------------------------------------------------------------------
IMPLEMENT_FUNCINFO(DetMatrixFunc);

DetMatrixFunc::DetMatrixFunc(void)
{ }

ResultTypeEnum DetMatrixFunc::GetReturnType(void) const
{
    return RT_RealVal;
}

FuncParamsInfo DetMatrixFunc::GetFuncInfo(void) const
{
    return FuncParamsInfo("Det", RT_Matrix);
}

Variable* DetMatrixFunc::Call(std::vector<Variable*> *pVarVec)
{
    MatrixVariable *pMatVar = (MatrixVariable*)(*pVarVec)[0];
    Matrix &matRef = pMatVar->GetValueRef();
    RealVal_t retRealVal = 0.0;

    if (matRef.Rows() != matRef.Cols())
        throw ExprException("Function Det just can be called against square Matrix.");

    retRealVal = ComputeDet(matRef);

    return (new RealVariable(retRealVal));
}

DetMatrixFunc::Row_Col_t DetMatrixFunc::TransCol(DetMatrixFunc::Row_Col_t colMask, DetMatrixFunc::Row_Col_t col) const
{
    Row_Col_t retCol = 0;

    for (Row_Col_t colFlag = 1; colFlag <= colMask; colFlag <<= 1) {
        if ((colFlag & colMask) == colFlag) {
            ++retCol;
        }
        else {
            if (col == 0) {
                break;
            }
            else {
                --col;
                ++retCol;
            }
        }
    }
    retCol += col;

    return retCol;
}

DetMatrixFunc::RealVal_t DetMatrixFunc::ComputeDet(const Matrix &matRef) const
{
    _ASSERT(matRef.Cols() == matRef.Rows());

    RealVal_t retRealVal = 0.0;

    if (matRef.Rows() <= 15) {
        RealValCell_t defaultRealCell = {false, 0.0};
        CacheVec_t cacheVec((1 << matRef.Rows()), defaultRealCell);

        retRealVal = ComputerDet_Help(matRef, cacheVec, 0, 0);
    }
    else {
        // TODO: Deal with large matrix.
        _ASSERT(0);
    }

    return retRealVal;
}

DetMatrixFunc::RealVal_t DetMatrixFunc::ComputerDet_Help(const Matrix &matRef, 
                                                         DetMatrixFunc::CacheVec_t &cacheVecRef, 
                                                         DetMatrixFunc::Row_Col_t rowCount, 
                                                         DetMatrixFunc::Row_Col_t colMask) const
{
    if (!cacheVecRef[colMask].bSet) {
        Row_Col_t rcCount = matRef.Rows() - rowCount;
        RealVal_t retRealVal = 0.0;
        Row_Col_t row = rowCount;
        
        if (rcCount > 1) {
            for (Row_Col_t rcIdx = 0; rcIdx < rcCount; ++rcIdx) {
                Row_Col_t col = TransCol(colMask, rcIdx);
                RealVal_t curRealVal = ((rcIdx & 1) == 0 ? matRef.At(row, col) : -1.0 * matRef.At(row, col)); 

                if (DOUBLE_NEQZ(curRealVal)) {
                    retRealVal += curRealVal * ComputerDet_Help(matRef, cacheVecRef, (rowCount + 1), 
                                                                (colMask | (1 << col)));
                }
                else {
                    continue;
                }
            }
        }
        else {
            retRealVal = matRef.At(row, TransCol(colMask, 0));
        }

        cacheVecRef[colMask].bSet = true;
        cacheVecRef[colMask].RealVal = retRealVal;
    }

    return cacheVecRef[colMask].RealVal;
}