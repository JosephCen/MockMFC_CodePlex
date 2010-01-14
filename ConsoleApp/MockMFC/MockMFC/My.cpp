#include "stdafx.h"
#include "My.h"
#include <iostream>
using std::cout;
using std::endl;

//----------------------------------------------------------------------------------------
// CMyWinApp - class member
//----------------------------------------------------------------------------------------
BOOL CMyWinApp::InitInstance()
{
   m_pMainWnd = new CMyFrameWnd();

   return TRUE;
}
//----------------------------------------------------------------------------------------
// CMyFrameWnd - class member
//----------------------------------------------------------------------------------------
CMyFrameWnd::CMyFrameWnd()
{
   cout << "CMyFrameWnd Constructor" << endl;
   Create();
}

void CMyFrameWnd::SayHello()
{
   std::cout << "Hello CMyFrameWnd" << std::endl;
}
//----------------------------------------------------------------------------------------
// CMyView - class member
//----------------------------------------------------------------------------------------
CMyView::CMyView()
{
   std::cout << "CMyView Constructor" << std::endl;
}

void CMyView::SayHello()
{
   std::cout << "Hello CMyView" << std::endl;
}
//----------------------------------------------------------------------------------------
// CMyDoc - class menber
//----------------------------------------------------------------------------------------
CMyDoc::CMyDoc()
{
   std::cout << "CMyDoc Constructor" << std::endl;
}

void CMyDoc::SayHello()
{
   std::cout << "Hello CMyDoc" << std::endl;
}
//----------------------------------------------------------------------------------------
// DYNAMIC & DYNCREATE implement
//----------------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CMyView, CView);
IMPLEMENT_DYNCREATE(CMyFrameWnd, CFrameWnd);
IMPLEMENT_DYNCREATE(CMyDoc, CDocument);
