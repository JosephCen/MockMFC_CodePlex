#ifndef FuncParamsInfo_H
#define FuncParamsInfo_H

#include <string>
#include <vector>

class FuncParamsInfo
{
private :
    std::string _FuncName;
    std::vector<int> _ParamTypes;
public :
    // Constructor
    FuncParamsInfo(const char *funcName, int paramType0, int paramType1);
    // Override Operators
    friend bool operator < (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal);
    friend bool operator == (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal);
};

bool operator < (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal);
bool operator == (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal);

#endif