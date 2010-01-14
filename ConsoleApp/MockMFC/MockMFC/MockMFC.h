#ifndef MockMFC_H
#define MockMFC_H

#include "MockMFCCore.h"

//----------------------------------------------------------------------------------------
// CCmdTarget - class
//----------------------------------------------------------------------------------------
class CCmdTarget : public CObject
{
   DECLARE_DYNAMIC(CCmdTarget)
public:
   CCmdTarget() { }
   virtual ~CCmdTarget() { }
};
//----------------------------------------------------------------------------------------
// CWinThread - class
//----------------------------------------------------------------------------------------
class CWinThread : public CCmdTarget
{
   DECLARE_DYNAMIC(CWinThread)
public:
   CWinThread() { }
   virtual ~CWinThread() { }

   virtual BOOL InitInstance();
   virtual int Run();
};
//----------------------------------------------------------------------------------------
// CWnd - class
//----------------------------------------------------------------------------------------
class CWnd : public CCmdTarget
{
   DECLARE_DYNCREATE(CWnd)
public:
   CWnd();
   virtual ~CWnd() { }

   virtual BOOL Create();
   BOOL CreateEx();
   virtual BOOL PreCreateWindow();
   virtual void SayHello();
};
//----------------------------------------------------------------------------------------
// CWinApp - class
//----------------------------------------------------------------------------------------
class CWinApp : public CWinThread
{
   DECLARE_DYNAMIC(CWinApp)
public:
   CWinApp* m_pCurrentWinApp;
   CWnd* m_pMainWnd;

public:
   CWinApp() : m_pCurrentWinApp(this) { }
   virtual ~CWinApp() { }

   virtual BOOL InitApplication();
   virtual BOOL InitInstance();
};
//----------------------------------------------------------------------------------------
// CDocument - class
//----------------------------------------------------------------------------------------
class CDocument : public CCmdTarget
{
   DECLARE_DYNAMIC(CDocument)
public:
   CDocument() { }
   virtual ~CDocument() { }
};
//----------------------------------------------------------------------------------------
// CFrameWnd - class
//----------------------------------------------------------------------------------------
class CFrameWnd : public CWnd
{
   DECLARE_DYNCREATE(CFrameWnd)
public:
   CFrameWnd();
   virtual ~CFrameWnd() { }

   virtual BOOL Create();
   virtual BOOL PreCreateWindow();
   virtual void SayHello();
};
//----------------------------------------------------------------------------------------
// CView - class
//----------------------------------------------------------------------------------------
class CView : public CWnd
{
   DECLARE_DYNAMIC(CView)
public:
   CView() { }
   virtual ~CView() { }
};

#endif