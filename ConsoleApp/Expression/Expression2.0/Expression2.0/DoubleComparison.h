#ifndef DoubleComparison_H
#define DoubleComparison_H

#define ERR_VAL 0.000001
#define DOUBLE_EQZ(val) ((val) >= -ERR_VAL && (val) <= ERR_VAL)
#define DOUBLE_NEQZ(val) ((val) < -ERR_VAL || (val) > ERR_VAL)
#define DOUBLE_EQ(val1, val2) (DOUBLE_EQZ((val1) - (val2)))
#define DOUBLE_NEQ(val1, val2) (DOUBLE_NEQZ((val1) - (val2)))
#define DOUBLE_LT(val1, val2) ((val2) - (val1) > ERR_VAL)
#define DOUBLE_GT(val1, val2) ((val1) - (val2) > ERR_VAL)

#endif