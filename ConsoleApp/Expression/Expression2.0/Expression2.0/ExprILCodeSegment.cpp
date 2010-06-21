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

bool ExprILCodeSegment::Run(VariableSet *pVariableSet, Variable **ppVariable)
{
    _ASSERT(NULL != pVariableSet);
    _ASSERT(NULL != ppVariable);

    bool state = true;
    ExprILRunState runState(pVariableSet, false);

    for (ILCodeIter_t codeIter = _ILCodeVec.begin(); state && _ILCodeVec.end() != codeIter; ++codeIter) {
        state &= (*codeIter)->RunCode(&runState);
    }

    if (state && 1 == runState.GetVariableStack()->Count())
        *ppVariable = runState.GetVariableStack()->DupTopVar();

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