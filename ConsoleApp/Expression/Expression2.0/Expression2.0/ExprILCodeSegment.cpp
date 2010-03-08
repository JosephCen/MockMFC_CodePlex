#include "stdafx.h"
#include "ExprILCodeSegment.h"
#include "ExprException.h"
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <crtdbg.h>

//---------------------------------------------------------------------
// Class member - ExprILCodeSegment
//---------------------------------------------------------------------
ExprILCodeSegment::ExprILCodeSegment() :
_ILCodeVec()
{ }

ExprILCodeSegment::~ExprILCodeSegment()
{
    for (ILCodeIter_t codeIter = _ILCodeVec.begin(); _ILCodeVec.end() != codeIter; ++codeIter)
        delete *codeIter;
}

bool ExprILCodeSegment::Run(Variable **ppVariable)
{
    bool state = true;
    VariableSet varSet(NULL, true); // Temp resolution
    ExprILRunState runState(&varSet, false);

    for (ILCodeIter_t codeIter = _ILCodeVec.begin(); state && _ILCodeVec.end() != codeIter; ++codeIter) {
        state &= (*codeIter)->RunCode(&runState);
    }

    if (state && 1 == runState.GetVariableStack()->Count())
        runState.GetVariableStack()->TopVar(ppVariable);

    return state;
}

void ExprILCodeSegment::ToString(ostringstream *pOStrStream) const
{
    *pOStrStream << "CodeSegment.Length: " << Length();
    for (ILCodeCIter_t codeIter = _ILCodeVec.begin(); _ILCodeVec.end() != codeIter; ++codeIter) {
        *pOStrStream << '\n';
        (*codeIter)->ToString(pOStrStream);
    }
}

string ExprILCodeSegment::ToString() const
{
    ostringstream oStrStream;

    ToString(&oStrStream);

    return oStrStream.str();
}