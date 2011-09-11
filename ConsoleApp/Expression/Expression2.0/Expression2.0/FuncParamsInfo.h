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
    FuncParamsInfo(const char *funcName, int paramType0);
    FuncParamsInfo(const char *funcName, int paramType0, int paramType1);
    FuncParamsInfo(const char *funcName, const std::vector<int> &paramVec);
    // Methods
    const std::string& GetFuncName(void) const;
    int GetParamCount(void) const;
    // Override Operators
    friend bool operator < (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal);
    friend bool operator == (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal);
};

bool operator < (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal);
bool operator == (const FuncParamsInfo &lVal, const FuncParamsInfo &rVal);

//---------------------------------------------------------------------
// Inline methods - FuncParamsInfo
//---------------------------------------------------------------------
inline const std::string& FuncParamsInfo::GetFuncName(void) const
{
    return _FuncName;
}

inline int FuncParamsInfo::GetParamCount(void) const
{
    return static_cast<int>(_ParamTypes.size());
}

#endif