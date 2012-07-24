#ifndef Symbol_H
#define Symbol_H

enum ExprILCodeEnum
{
    EIL_NUL = -1,
    EIL_PushInteger,
    EIL_PushRealVal,
    EIL_PushDefVal,
    EIL_RealValPlus,
    EIL_RealValMinus,
    EIL_RealValMultiply,
    EIL_RealValDivide,
    EIL_CtorMatrix,
    EIL_MatrixPlus,
    EIL_MatrixMinus,
    EIL_MatrixMultiply,
    EIL_MatrixDotMultiply,
    EIL_MatrixDivide,
    EIL_MatrixDotDivide,
    EIL_MatrixValPlus,
    EIL_MatrixValMinus,
    EIL_MatrixValMultiply,
    EIL_MatrixValDivide,
    EIL_CallFunction,
    EIL_NewVariable,
    //
    EIL_Count,
};

enum ResultTypeEnum
{
    RT_None,
    RT_String,
    RT_Matrix,
    RT_Integer,
    RT_RealVal
};

#endif