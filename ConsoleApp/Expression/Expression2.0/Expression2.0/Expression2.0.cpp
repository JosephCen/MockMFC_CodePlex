// Expression2.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ExprWorkSpace.h"
#include "VariableSet.h"
#include "Formatter.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <string>
using std::string;
using std::getline;
#include <exception>
using std::exception;

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

int _tmain(int argc, _TCHAR* argv[])
{
    ExprWorkSpace curWorkSpace;
    Formatter formatter;
    string inputStr;
    string errorStr;
    string resultStr;
    ExprILCodeSegment *pILCodeSegment = NULL;
    Variable *pVariable = NULL;

    while (getline(cin, inputStr))
    {
        if (curWorkSpace.ParseILCodeSegment(inputStr, &pILCodeSegment))
        {
            if (pILCodeSegment->Run(&pVariable))
            {
                formatter.FormatVariable(pVariable, &resultStr);
                cout << "Result: " << endl;
                cout << resultStr << endl;

                pVariable = NULL;
            }
            else
            {
                pILCodeSegment->GetExceptionStr(&errorStr);
                cout << "Execute Error: " << endl;
                cout << errorStr << endl;
            }
        }
        else
        {
            curWorkSpace.GetExceptionStr(&errorStr);
            cout << "Parse Error: " << endl;
            cout << errorStr << endl;
        }
    }
    cin.clear();

	return 0;
}

