#ifndef MockMFCCore_H
#define MockMFCCore_H

typedef int BOOL;
const BOOL TRUE = 1;
const BOOL FALSE = 0;

typedef int UINT;
typedef char* LPSTR;

#define PASCAL _stdcall

#include <iostream>

class CObject;

struct CRuntimeClass
{
   LPSTR m_lpszClassName;
   int m_nObjectSize;
   UINT m_wSchema;
   CObject* (PASCAL* m_pfnCreateObject)();
   CRuntimeClass* m_pBaseClass;
   CRuntimeClass* m_pNextClass;

   CObject* CreateObject();

   static CRuntimeClass* mS_pFirstClass;

   static CRuntimeClass* PASCAL GetRuntimeClass(const char *lpszClsName);
};

struct AFX_CLASSINIT
{
   AFX_CLASSINIT(CRuntimeClass* pNewClass);
};

#define RUNTIME_CLASS(class_name) \
   (&class_name::class##class_name)

#define DECLARE_DYNAMIC(class_name) \
   public: \
      static CRuntimeClass class##class_name; \
      virtual CRuntimeClass* GetRuntimeClass() const;

#define DECLARE_DYNCREATE(class_name) \
   DECLARE_DYNAMIC(class_name) \
   static CObject* PASCAL CreateObject();

#define _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
   static char lpsz##class_name[] = #class_name; \
   CRuntimeClass class_name::class##class_name = { \
      lpsz##class_name, sizeof(class_name), wSchema, pfnNew, RUNTIME_CLASS(base_class_name), NULL \
      }; \
   static AFX_CLASSINIT _init_##class_name(&class_name::class##class_name); \
   CRuntimeClass* class_name::GetRuntimeClass() const \
   { return &class_name::class##class_name; }

#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
   _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL)

#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
   CObject* PASCAL class_name::CreateObject() \
   { return new class_name(); } \
   _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, class_name::CreateObject)

//----------------------------------------------------------------------------------------
// CObject - class
//----------------------------------------------------------------------------------------
class CObject
{
public:
   CObject() { }
   virtual ~CObject() { }

   virtual CRuntimeClass* GetRuntimeClass() const;
   BOOL IsKindOf(const CRuntimeClass* pClass) const;
public:
   static CRuntimeClass classCObject;
   virtual void SayHello();
};

#endif