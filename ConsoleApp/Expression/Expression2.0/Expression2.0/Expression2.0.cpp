// Expression2.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ExprWorkSpace.h"
#include "ExprILCodeSegment.h"
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
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
//    ~TestCls() { cout << "Call Destructor." << "IsCopy: " << _IsCopy << endl; }
//private :
//    
//};
//
//TestCls TestFn()
//{
//    TestCls cls = TestCls(5);
//
//    return cls;
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
    string inputStr("[(123.4+55.78), 5.7, 11; 1.2-0.8, 2.1, 1.1].* [12,+56.2, 1.3;-0.34, 12.0, 1.11]");
    ExprILCodeSegment ilCodeSegment;
    
    if (curWorkSpace.ParseILCodeSegment(inputStr, &ilCodeSegment)) {
        cout << ilCodeSegment.ToString() << endl;
    }

	return 0;
}

