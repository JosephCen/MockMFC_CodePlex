// program.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MockMFC.h"
#include "My.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <string>
using std::getline;
using std::string;

//----------------------------------------------------------------------------------------
// PrintAllClasses - global function 
//----------------------------------------------------------------------------------------
void PrintAllClasses()
{
   int classCount = 0;

   for (CRuntimeClass* pClass = CRuntimeClass::mS_pFirstClass; NULL != pClass; pClass = pClass->m_pNextClass)
   {
      ++classCount;
      
      cout << pClass->m_lpszClassName << endl;
      if (NULL != pClass->m_pBaseClass)
         cout << "   Base Class: " << pClass->m_pBaseClass->m_lpszClassName << endl;
      cout << "   " << pClass->m_nObjectSize << endl;
      cout << "   Support DynCreate: " << ( NULL != pClass->m_pfnCreateObject ? "True" : "False" ) << endl;
      cout << endl;
   }
   cout << "Total " << classCount << " class(es)." << endl;
}
//----------------------------------------------------------------------------------------
// DynamicCreate - global function 
//----------------------------------------------------------------------------------------
void DynamicCreate()
{
   string inStr;
   CRuntimeClass *pRuntimeCls = NULL;

   while (1)
   {
      cout << "Input Class Name: " << endl;
      getline(cin, inStr);

      pRuntimeCls = CRuntimeClass::GetRuntimeClass(inStr.c_str());
      if (NULL != pRuntimeCls)
      {
         if (NULL != pRuntimeCls->m_pfnCreateObject)
         {
            CObject *pCObj = NULL;
            
            cout << "Dynamic create class '" << pRuntimeCls->m_lpszClassName << "'." << endl;
            pCObj = pRuntimeCls->CreateObject();
            pCObj->SayHello();
            delete pCObj;
            cout << endl;
         }
         else
         {
            cout << "Class '" << pRuntimeCls->m_lpszClassName << "' do not support dynamic creat." << endl;
            cout << endl;
         }
      }
      else
      {
         cout << "Class is not existing!" << endl;
         cout << "Press 'Enter' to exit." << endl;
         getline(cin, inStr);
         cout << endl;
         break;
      }
   }
}

int _tmain(int argc, _TCHAR* argv[])
{
   PrintAllClasses();
   DynamicCreate();

	return 0;
}

