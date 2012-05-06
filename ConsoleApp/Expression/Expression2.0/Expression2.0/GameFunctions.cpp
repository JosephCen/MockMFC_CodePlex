#include "stdafx.h"
#include "DoubleComparison.h"
#include "ExprException.h"
#include "GameFunctions.h"
#include <vector>
#include <crtdbg.h>
using namespace std;

//---------------------------------------------------------------------
// Point24 - class
//---------------------------------------------------------------------
struct SubExprInfo
{
    string SubExprStr;
    char OperCh;
    double Number;
};

class Point24
{
private :
    static int s_OperPriTab[48];
    static SubExprInfo s_EmptySubExpr;
    double _FinalVal;
    vector<SubExprInfo> _SubExprVec;
public :
    // Constructor
    Point24(const vector<double> &dblVecRef, double finalVal);
    // Methods
    bool Calculate(string &exprStrRef);
private :
    bool Calculate_Impl(size_t n);
    static void BuildExpr(SubExprInfo &o, SubExprInfo &a, SubExprInfo &b, char operCh);
};

//---------------------------------------------------------------------
// Class member - Point24
//---------------------------------------------------------------------
int Point24::s_OperPriTab[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    3, // s_OperPriTab[' '] = 3
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, // s_OperPriTab['*'] = 2
    1, // s_OperPriTab['+'] = 1
    0, 
    1, // s_OperPriTab['-'] = 1
    0,
    2  // s_OperPriTab['/'] = 2
};

SubExprInfo Point24::s_EmptySubExpr = {
    string(""), ' ', 0.0
};

Point24::Point24(const vector<double> &dblVecRef, double finalVal) :
_SubExprVec(dblVecRef.size(), s_EmptySubExpr), _FinalVal(finalVal)
{
    _ASSERT(dblVecRef.size() > 0);

    ostringstream strStream;
    
    for (size_t i = 0; i < dblVecRef.size(); ++i) {
        strStream.str("");
        strStream << dblVecRef[i];

        _SubExprVec[i].SubExprStr = strStream.str();
        _SubExprVec[i].Number = dblVecRef[i];
    }
}

bool Point24::Calculate(string &exprStrRef)
{
    if (Calculate_Impl(_SubExprVec.size())) {
        exprStrRef = _SubExprVec[0].SubExprStr;

        return true;
    }

    return false;
}

void Point24::BuildExpr(SubExprInfo &o, SubExprInfo &a, SubExprInfo &b, char operCh)
{
    int a_OpPri = s_OperPriTab[a.OperCh];
    int b_OpPri = s_OperPriTab[b.OperCh];
    int cur_OpPri = s_OperPriTab[operCh];

    o.OperCh = operCh;

    o.SubExprStr.clear();
    if (a_OpPri < cur_OpPri)
        o.SubExprStr.append(1, '(').append(a.SubExprStr).append(1, ')');
    else
        o.SubExprStr.append(a.SubExprStr);

    o.SubExprStr.append(1, operCh);

    if (b_OpPri <= cur_OpPri)
        o.SubExprStr.append(1, '(').append(b.SubExprStr).append(1, ')');
    else
        o.SubExprStr.append(b.SubExprStr);
}

bool Point24::Calculate_Impl(size_t n)
{
    if (1 == n)
        return DOUBLE_EQ(_SubExprVec[0].Number, _FinalVal);
    
    for (size_t i = 0; i < _SubExprVec.size(); ++i) {
        for (size_t j = i + 1; j < _SubExprVec.size(); ++j) {
            SubExprInfo subExprA, subExprB;

            subExprA = _SubExprVec[i];
            subExprB = _SubExprVec[j];
            _SubExprVec[j] = _SubExprVec[n - 1];

            _SubExprVec[i].Number = subExprA.Number + subExprB.Number;
            BuildExpr(_SubExprVec[i], subExprA, subExprB, '+');
            if (Calculate_Impl(n - 1))
                return true;

            _SubExprVec[i].Number = subExprA.Number * subExprB.Number;
            BuildExpr(_SubExprVec[i], subExprA, subExprB, '*');
            if (Calculate_Impl(n - 1))
                return true;

            _SubExprVec[i].Number = subExprA.Number - subExprB.Number;
            BuildExpr(_SubExprVec[i], subExprA, subExprB, '-');
            if (Calculate_Impl(n - 1))
                return true;

            if (DOUBLE_NEQZ(subExprB.Number)) {
                _SubExprVec[i].Number = subExprA.Number / subExprB.Number;
                BuildExpr(_SubExprVec[i], subExprA, subExprB, '/');
                if (Calculate_Impl(n - 1))
                    return true;
            }

            if (DOUBLE_NEQ(subExprA.Number, subExprB.Number)) {
                _SubExprVec[i].Number = subExprB.Number - subExprA.Number;
                BuildExpr(_SubExprVec[i], subExprB, subExprA, '-');
                if (Calculate_Impl(n - 1))
                    return true;

                if (DOUBLE_NEQZ(subExprA.Number)) {
                    _SubExprVec[i].Number = subExprB.Number / subExprA.Number;
                    BuildExpr(_SubExprVec[i], subExprB, subExprA, '/');
                    if (Calculate_Impl(n - 1))
                        return true;
                }
            }

            _SubExprVec[i] = subExprA;
            _SubExprVec[j] = subExprB;
        }
    }

    return false;
}

//---------------------------------------------------------------------
// Class member - Point24Func
//---------------------------------------------------------------------
IMPLEMENT_FUNCINFO(Point24Func);

Point24Func::Point24Func(void)
{ }

ResultTypeEnum Point24Func::GetReturnType(void) const
{
    return RT_String;
}

FuncParamsInfo Point24Func::GetFuncInfo(void) const
{
    return FuncParamsInfo("Point24", RT_Matrix, RT_RealVal);
}

Variable* Point24Func::Call(std::vector<Variable*> *pVarVec)
{
    MatrixVariable *pMatVar = (MatrixVariable*)(*pVarVec)[0];
    RealVariable *pRealVar = (RealVariable*)(*pVarVec)[1];
    string resultStr;

    Calculate(pMatVar->GetValueRef(), pRealVar->GetValue(), resultStr);

    return (new StrVariable(resultStr));
}

void Point24Func::Calculate(const Matrix &matRef, Matrix::RealVal_t finalVal, std::string &resultStrRef)
{
    if ((matRef.Rows() + matRef.Cols()) > 0) {
        vector<double> dblVec;

        for (Matrix::Row_Col_t r = 0; r < matRef.Rows(); ++r) {
            for (Matrix::Row_Col_t c = 0; c < matRef.Cols(); ++c)
                dblVec.push_back(matRef.At(r, c));
        }

        Point24 p24(dblVec, finalVal);

        if (!p24.Calculate(resultStrRef))
            resultStrRef = "No Solution";
    }
    else {
        resultStrRef = "No avaliable number!";
    }
}