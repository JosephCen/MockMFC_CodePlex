#ifndef DoubleComparison_H
#define DoubleComparison_H

const double ERR_VAL = 0.000001;

inline bool DOUBLE_EQZ(double val)
{
    return (val >= -ERR_VAL && val <= ERR_VAL);
}

inline bool DOUBLE_NEQZ(double val)
{
    return (val < -ERR_VAL || val > ERR_VAL);
}

inline bool DOUBLE_EQ(double val1, double val2)
{
    return (DOUBLE_EQZ(val1 - val2));
}

inline bool DOUBLE_NEQ(double val1, double val2)
{
    return (DOUBLE_NEQZ(val1 - val2));
}

inline bool DOUBLE_LT(double val1, double val2)
{
    return (val2 - val1 > ERR_VAL);
}

inline bool DOUBLE_GT(double val1, double val2)
{
    return (val1 - val2 > ERR_VAL);
}

inline int DOUBLE_COMP(double val1, double val2)
{
    double diff = val1 - val2;

    return ((diff > ERR_VAL ? 1 : (diff < -ERR_VAL ? -1 : 0)));
}

#endif