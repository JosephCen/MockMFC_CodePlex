#include "stdafx.h"
#include "ExprWorkSpace.h"
#include "VariableSet.h"
#include "ExprAdapter.h"
#include "WordParser.h"
#include "BaseNonTerminal.h"
#include "ExprILCode.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <sstream>
#include <crtdbg.h>
using namespace std;


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
    for_each(_ILCodeVec.begin(), _ILCodeVec.end(), [](ExprILCode* &pILRef) {
        delete pILRef;
        pILRef = nullptr;
    });
    _ILCodeVec.clear();
    _pWorkSpace->RemoveILCodeSegment(this);
    _pWorkSpace = NULL;
}

Variable_sp ExprILCodeSegment::Run()
{
    bool state = true;
    Variable_sp spVariable;
    ExprILRunState runState(&(_pWorkSpace->_GlobalVarSet), false); // Use friend declaration

    // Clear all existing Error info
    this->ClearError();
    for (auto codeIter = _ILCodeVec.begin(); state && _ILCodeVec.end() != codeIter; ++codeIter) {
        state &= (*codeIter)->RunCode(&runState);
    }

    if (state) {
        if (1 == runState.GetVariableStack()->Count()) {
            spVariable = Variable_sp(runState.GetVariableStack()->DupTopVar());
            _pWorkSpace->_GlobalVarSet.InsertVar("Ans", spVariable);
        }
        // TODO: add else part (Given a default empty return Variable)
    }
    else {
        // Collect all error info generate during run
        this->MergeError(runState);
    }

    return spVariable;
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
{
    _WordParser.SetExprWorkSpace(this);
}

ExprWorkSpace::~ExprWorkSpace()
{
    _IsDesructing = true;
    _ILCodeSegmentSet.clear();
}

ExprILCodeSegment_sp ExprWorkSpace::ParseILCodeSegment(const std::string &codeStr)
{
    WordFwCursor wordCursor = _WordParser.GenWordFwCursor(codeStr);
    ExprContext exprContext;

    // Clear all existing Error info
    this->ClearError();
    if (wordCursor.NextWord(exprContext)) {
        unique_ptr<BaseNonTerminal> upStartNT(BaseNonTerminal::NewStartNT());

        if (upStartNT->Parse(exprContext, wordCursor)) {
            ExprILCodeSegment_sp spILSegment = ExprILCodeSegment_sp(new ExprILCodeSegment(this));

            upStartNT->AppendILSegment(*spILSegment);
            if (spILSegment->Length() > 0) {
                _ILCodeSegmentSet.push_back(spILSegment);

                return spILSegment;
            }
            else {
                this->SetError("Empty code string");
            }
        }
    }
    // Collect all error info generate during run
    this->MergeError(exprContext);

    return (ExprILCodeSegment_sp());
}

Variable_sp ExprWorkSpace::RunILCodeSegment(ExprILCodeSegment_sp spILSegment)
{
    _ASSERT((bool)spILSegment);

    Variable_sp spVariable;

    // Clear all existing Error info
    this->ClearError();
    if (!(spVariable = spILSegment->Run()))
        this->MergeError(spILSegment.get());

    return spVariable;
}

void ExprWorkSpace::RemoveILCodeSegment(ExprILCodeSegment *pILSegment)
{
    _ASSERT(nullptr != pILSegment);

    if (!_IsDesructing)
    {
        ILCodeSegmentIter_t iter = _ILCodeSegmentSet.begin();

        iter = find_if(iter, _ILCodeSegmentSet.end(), [pILSegment](ExprILCodeSegment_sp &spILSegment) -> bool {
			return (spILSegment.get() == pILSegment);
		});
        if (_ILCodeSegmentSet.end() != iter)
            _ILCodeSegmentSet.erase(iter);
    }
}