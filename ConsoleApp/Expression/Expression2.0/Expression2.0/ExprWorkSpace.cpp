#include "stdafx.h"
#include "ExprWorkSpace.h"
#include "VariableSet.h"
#include "ExprAdapter.h"
#include "WordParser.h"
#include "NonTerminal.h"
#include "ExprILCode.h"
#include <vector>
using std::vector;
#include <algorithm>
using std::find;
#include <string>
using std::string;
#include <sstream>
using std::ostream;
using std::ostringstream;
#include <crtdbg.h>


//---------------------------------------------------------------------
// Class member - ExprILCodeSegment
//---------------------------------------------------------------------
ExprILCodeSegment::ExprILCodeSegment(ExprWorkSpace *pWorkSpace) :
ExprErrHolder(), _pWorkSpace(pWorkSpace), _ILCodeVec()
{
    _ASSERT(NULL != pWorkSpace);
}

ExprILCodeSegment::~ExprILCodeSegment()
{
    _pWorkSpace->RemoveILCodeSegment(this);
    for (ILCodeIter_t codeIter = _ILCodeVec.begin(); _ILCodeVec.end() != codeIter; ++codeIter)
        delete *codeIter;
}

bool ExprILCodeSegment::Run(Variable **ppVariable)
{
    _ASSERT(NULL != ppVariable);

    bool state = true;
    ExprILRunState runState(&(_pWorkSpace->_GlobalVarSet), false); // Use friend declaration

    // Clear all existing Error info
    this->ClearError();
    for (ILCodeIter_t codeIter = _ILCodeVec.begin(); state && _ILCodeVec.end() != codeIter; ++codeIter) {
        state &= (*codeIter)->RunCode(&runState);
    }

    if (state)
    {
        if (1 == runState.GetVariableStack()->Count())
            *ppVariable = runState.GetVariableStack()->DupTopVar();
        // TODO: add else part (Given a default empty return Variable)
    }
    else
    {
        // Collect all error info generate during run
        this->MergeError(runState);
    }

    return state;
}

void ExprILCodeSegment::ToString(ostream *pOStream) const
{
    *pOStream << "CodeSegment.Length: " << Length();
    for (ILCodeCIter_t codeIter = _ILCodeVec.begin(); _ILCodeVec.end() != codeIter; ++codeIter) {
        *pOStream << '\n';
        (*codeIter)->ToString(pOStream);
    }
}

string ExprILCodeSegment::ToString() const
{
    ostringstream oStrStream;

    ToString(&oStrStream);

    return oStrStream.str();
}

//---------------------------------------------------------------------
// Class member - ExprWorkSpace
//---------------------------------------------------------------------
ExprWorkSpace::ExprWorkSpace() :
ExprErrHolder(), _GlobalVarSet(), _WordParser(), _ILCodeSegmentSet(), _IsDesructing(false)
{ }

ExprWorkSpace::~ExprWorkSpace()
{
    _IsDesructing = true;
    for (ILCodeSegmentIter_t iter = _ILCodeSegmentSet.begin(); _ILCodeSegmentSet.end() != iter; ++iter)
        delete *iter;
    _ILCodeSegmentSet.clear();
}

bool ExprWorkSpace::ParseILCodeSegment(const std::string &codeStr, ExprILCodeSegment **ppILSegment)
{
    _ASSERT(NULL != ppILSegment);

    WordFwCursor wordCursor = _WordParser.GenWordFwCursor(codeStr);
    ExprContext exprContext;

    // Clear all existing Error info
    this->ClearError();
    if (wordCursor.NextWord(exprContext)) {
        StartNT startNT;

        if (startNT.Parse(exprContext, wordCursor)) {
            ExprILCodeSegment *pILSegment = new ExprILCodeSegment(this);

            _ILCodeSegmentSet.push_back(pILSegment);
            startNT.AppendILSegment(*pILSegment);
            *ppILSegment = pILSegment;

            return true;
        }
    }
    // Collect all error info generate during run
    this->MergeError(exprContext);

    return false;
}

bool ExprWorkSpace::RunILCodeSegment(ExprILCodeSegment *pILSegment, Variable **ppVariable)
{
    _ASSERT(NULL != pILSegment);
    _ASSERT(NULL != ppVariable);

    // Clear all existing Error info
    this->ClearError();
    if (pILSegment->Run(ppVariable))
    {
        return true;
    }
    else
    {
        this->MergeError(pILSegment);

        return false;
    }
}

void ExprWorkSpace::RemoveILCodeSegment(ExprILCodeSegment *pILSegment)
{
    _ASSERT(NULL != pILSegment);

    if (!_IsDesructing)
    {
        ILCodeSegmentIter_t iter = _ILCodeSegmentSet.begin();

        iter = find(iter, _ILCodeSegmentSet.end(), pILSegment);
        if (_ILCodeSegmentSet.end() != iter)
            _ILCodeSegmentSet.erase(iter);
    }
}