// Expression2.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ExprWorkSpace.h"
#include "VariableSet.h"
#include "Formatter.h"
#include "ExprILHelper.h"
#include <iostream>
#include <string>
#include <exception>
using namespace std;

//class TestCls
//{
//private :
//    int _a;
//    bool _IsCopy;
//public :
//    TestCls(int val_a);
//    TestCls();
//    TestCls(const TestCls &ref);
//    virtual int Method() { return _a++; }
//    operator bool() const { return 0 == _a; }
//    ~TestCls() { cout << "Call Destructor." << "IsCopy: " << (_IsCopy ? "True" : "False") << endl; }
//private :
//    
//};
//
//TestCls TestCopy1()
//{
//    TestCls cls = TestCls(5);
//
//    return cls;
//}
//
//TestCls TestCopy2()
//{
//    return TestCls(4);
//}
//
//void TestCopy3(TestCls cls)
//{
//    cls.Method();
//}
//
//void TestFun()
//{
//    TestCls cls1 = TestCopy1();
//    TestCls cls2 = TestCopy2();
//
//    TestCopy1();
//    TestCopy2();
//
//    TestCopy3(cls2);
//}
//
//TestCls::TestCls(int val_a) :
//_a(val_a), _IsCopy(false)
//{
//    cout << "Call Constructor." << endl;
//}
//
//TestCls::TestCls() :
//_a(0), _IsCopy(false)
//{
//    cout << "Call Constructor." << endl;
//}
//
//TestCls::TestCls(const TestCls &ref) :
//_a(ref._a), _IsCopy(true)
//{
//    cout << "Call Copy Constructor." << endl;
//}

void RunApplication();

int _tmain(int argc, _TCHAR* argv[])
{
	RunApplication();

	return 0;
}

void RunApplication()
{
	ExprWorkSpace curWorkSpace;
    Formatter formatter;
    string inputStr;
    string errorStr;
    string resultStr;
    ExprILCodeSegment_sp spILCodeSegment;
    Variable_sp spVariable;

    while (getline(cin, inputStr))
    {
        if ((bool)(spILCodeSegment = curWorkSpace.ParseILCodeSegment(inputStr)))
        {
            if ((bool)(spVariable = spILCodeSegment->Run()))
            {
                formatter.FormatVariable(spVariable.get(), &resultStr);
                cout << "Result: " << endl;
                cout << resultStr << '\n' << endl;
            }
            else
            {
                spILCodeSegment->GetExceptionStr(&errorStr);
                cout << "Execute Error: " << endl;
                cout << errorStr << '\n' << endl;
            }
        }
        else
        {
            curWorkSpace.GetExceptionStr(&errorStr);
            cout << "Parse Error: " << endl;
            cout << errorStr << '\n' << endl;
        }
    }

	ExprILHelper::ReleaseRes();
    cin.clear();
}