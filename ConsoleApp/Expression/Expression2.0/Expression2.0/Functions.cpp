#include "stdafx.h"
#include "Functions.h"
using namespace std;


IMPLEMENT_FUNCINFO(EMatrixFunc);

EMatrixFunc::EMatrixFunc(void) :
BaseFunction(FuncParamsInfo("E", RT_RealVal))
{ }

ResultTypeEnum EMatrixFunc::GetReturnType(void) const
{
    return RT_Matrix;
}

Variable* EMatrixFunc::Call(std::vector<Variable*> *pVarVec)
{
    RealVariable *pRealVar = (RealVariable*)(*pVarVec)[0];
    Matrix::RealVal_t realVal = pRealVar->GetValue();
    Matrix::Row_Col_t rowCol = static_cast<Matrix::Row_Col_t>(realVal);

    return (new MatrixVariable(Matrix(rowCol, rowCol, 'E')));
}