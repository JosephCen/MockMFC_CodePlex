// Expression2.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WordParser.h"
#include "NonTerminal.h"
#include "ParameterSet.h"
#include "ExprAdapter.h"
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
    string inputStr("[(123.4 +55.78), 5.7, 11; 1.2-0.8, 2.1, 1.1].* [12,+56.2, 1.3;-0.34, 12.0, 1.11]");
    //WordParser parser(inputStr);
    //WordUnit curWordUnit;
    //StartNT startNT;
    ExprAdapter exprAdapter;

    if (exprAdapter.Parse(inputStr)) {
        cout << exprAdapter.GetILSegment().ToString() << endl;
    }
    else {
        if (exprAdapter.GetException().ErrorChIdx() >= 0) {
            cout << "Error at: " << exprAdapter.GetException().ErrorChIdx() << endl;
            cout << inputStr << endl;
            cout << string(exprAdapter.GetException().ErrorChIdx(), ' ') << '^' << endl;
        }
        cout << exprAdapter.GetException().what() << endl;
    }


    /*if (parser.NextWord()) {
        if (startNT.Parse(parser))
            cout << startNT.ILSegment().ToString() << endl;
        else
            cout << startNT.GetException().what() << endl;
    }*/

    //ExprRunTime runTime(startNT.ILSegment());
    //runTime.Run();

    //while (curWordUnit = parser.NextWord()) {
    //    cout << curWordUnit.ToString() << endl;
    //}

    //if (!parser) {
    //    cout << inputStr << endl;
    //    cout << parser.ErrorMsg() << endl;
    //}

	return 0;
}

