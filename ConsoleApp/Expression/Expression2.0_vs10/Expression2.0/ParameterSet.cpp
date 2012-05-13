#include "stdafx.h"
#include "ParameterSet.h"
#include "Matrix.h"
#include <string>
using std::string;
#include <crtdbg.h>

// Member of class 'Parameter'

Parameter::Parameter(const Parameter &param)
{
    _Type = param._Type;
    switch (_Type) {
        case PT_String :
            _pStr = new string(*param._pStr);
            break;
        case PT_RealVal :
            _RealVal = param._RealVal;
            break;
        case PT_Integer :
            _Int = param._Int;
            break;
        case PT_Matrix :
            _pMatrix = new Matrix(*param._pMatrix);
            break;
    }
}

Parameter::~Parameter()
{
    switch (_Type) {
        case PT_String :
            delete _pStr;
            break;
        case PT_Matrix :
            delete _pMatrix;
            break;
    }
}

Parameter& Parameter::operator =(const Parameter &param)
{
    switch (_Type) {
        case PT_String :
            delete _pStr;
            break;
        case PT_Matrix :
            delete _pMatrix;
            break;
    }
    _Type = param._Type;
    switch (_Type) {
        case PT_String :
            _pStr = new string(*param._pStr);
            break;
        case PT_RealVal :
            _RealVal = param._RealVal;
            break;
        case PT_Integer :
            _Int = param._Int;
            break;
        case PT_Matrix :
            _pMatrix = new Matrix(*param._pMatrix);
            break;
    }

    return *this;
}