#ifndef My_H
#define My_H

#include "MockMFC.h"

//----------------------------------------------------------------------------------------
// CMyWinApp - class
//----------------------------------------------------------------------------------------
class CMyWinApp : public CWinApp
{
public:
   CMyWinApp() { }
   virtual ~CMyWinApp() { }

   virtual BOOL InitInstance();
};
//----------------------------------------------------------------------------------------
// CMyFrameWnd - class
//----------------------------------------------------------------------------------------
class CMyFrameWnd : public CFrameWnd
{
   DECLARE_DYNCREATE(CMyFrameWnd)
public:
   CMyFrameWnd();
   virtual ~CMyFrameWnd() { }

   virtual void SayHello();
};
//----------------------------------------------------------------------------------------
// CMyDoc - class
//----------------------------------------------------------------------------------------
class CMyDoc : public CDocument
{
   DECLARE_DYNCREATE(CMyDoc)
public:
   CMyDoc();
   virtual ~CMyDoc() { }

   virtual void SayHello();
};
//----------------------------------------------------------------------------------------
// CMyView - class
//----------------------------------------------------------------------------------------
class CMyView : public CView
{
   DECLARE_DYNCREATE(CMyView)
public:
   CMyView();
   virtual ~CMyView() { }

   virtual void SayHello();
};

#endif