#include "stdafx.h"
#include "MockMFC.h"
#include <string.h>

//----------------------------------------------------------------------------------------
// CRuntimeClass - class member
//----------------------------------------------------------------------------------------
CRuntimeClass* CRuntimeClass::mS_pFirstClass = NULL;

CObject* CRuntimeClass::CreateObject()
{
   _ASSERT(NULL != m_pfnCreateObject);

   return m_pfnCreateObject();
}

CRuntimeClass* CRuntimeClass::GetRuntimeClass(const char *lpszClsName)
{
   for (CRuntimeClass* pClass = CRuntimeClass::mS_pFirstClass; NULL != pClass; pClass = pClass->m_pNextClass)
   {
      if (0 == strcmp(pClass->m_lpszClassName, lpszClsName))
         return pClass;
   }

   return NULL;
}
//----------------------------------------------------------------------------------------
// AFX_CLASSINIT - class member
//----------------------------------------------------------------------------------------
AFX_CLASSINIT::AFX_CLASSINIT(CRuntimeClass *pNewClass)
{
   pNewClass->m_pNextClass = CRuntimeClass::mS_pFirstClass;
   CRuntimeClass::mS_pFirstClass = pNewClass;
}
//----------------------------------------------------------------------------------------
// CObject - class member
//----------------------------------------------------------------------------------------
static char lpszCObject[] = "CObject";

struct CRuntimeClass CObject::classCObject = {
   lpszCObject, sizeof(CObject), 0xffff, NULL, NULL
};

static AFX_CLASSINIT _init_CObject(RUNTIME_CLASS(CObject));

CRuntimeClass* CObject::GetRuntimeClass() const
{
   return RUNTIME_CLASS(CObject);
}

BOOL CObject::IsKindOf(const CRuntimeClass *pClass) const
{
   for (CRuntimeClass *pCurClass = GetRuntimeClass(); NULL != pCurClass; pCurClass = pCurClass->m_pBaseClass)
   {
      if (pCurClass == pClass)
         return TRUE;
   }

   return FALSE;
}

void CObject::SayHello()
{
   std::cout << "Hello CObject." << std::endl;
}
//----------------------------------------------------------------------------------------
// CWinThread - class member
//----------------------------------------------------------------------------------------
BOOL CWinThread::InitInstance()
{
   return TRUE;
}

int CWinThread::Run()
{
   return 1;
}
//----------------------------------------------------------------------------------------
// CWinApp - class member
//----------------------------------------------------------------------------------------
BOOL CWinApp::InitApplication()
{
   return TRUE;
}

BOOL CWinApp::InitInstance()
{
   return TRUE;
}
//----------------------------------------------------------------------------------------
// CWnd - class member
//----------------------------------------------------------------------------------------
CWnd::CWnd()
{
   std::cout << "CWnd Constructor" << std::endl;
}

void CWnd::SayHello()
{
   std::cout << "Hello CWnd" << std::endl;
}

BOOL CWnd::Create()
{
   return TRUE;
}

BOOL CWnd::CreateEx()
{
   PreCreateWindow();

   return TRUE;
}

BOOL CWnd::PreCreateWindow()
{
   return TRUE;
}
//----------------------------------------------------------------------------------------
// CFrameWnd - class member
//----------------------------------------------------------------------------------------
CFrameWnd::CFrameWnd()
{
   std::cout << "CFrameWnd Constructor" << std::endl;
}
void CFrameWnd::SayHello()
{
   std::cout << "Hello CFrameWnd" << std::endl;
}

BOOL CFrameWnd::Create()
{
   CreateEx();

   return TRUE;
}

BOOL CFrameWnd::PreCreateWindow()
{
   return TRUE;
}
//----------------------------------------------------------------------------------------
// DYNAMIC & DYNCREATE implement
//----------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CCmdTarget, CObject);
IMPLEMENT_DYNAMIC(CWinThread, CCmdTarget);
IMPLEMENT_DYNAMIC(CWinApp, CWinThread);
IMPLEMENT_DYNCREATE(CWnd, CCmdTarget);
IMPLEMENT_DYNAMIC(CView, CWnd);
IMPLEMENT_DYNCREATE(CFrameWnd, CWnd);
IMPLEMENT_DYNAMIC(CDocument, CCmdTarget);
