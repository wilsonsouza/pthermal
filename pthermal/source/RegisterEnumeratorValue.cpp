/*
*  Layer to load USB details
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 2 2016
*  Last updated
*  Version 0.1-beta
*/
#include <RegisterEnumeratorKey.h>
#include <RegisterEnumeratorValue.h>
#include <HKeyLocalMachine.h>
#include <HKeyLocalMachineSuccess.h>
using namespace reg;
//-------------------------------------------------------------------------------------------//
RegisterEnumeratorValue::RegisterEnumeratorValue(IRegisterEnumeratorKey const & in):
RegisterEnumeratorKey(in)
{
   clear();
}
//-------------------------------------------------------------------------------------------//
RegisterEnumeratorValue::~RegisterEnumeratorValue()
{
   clear();
}
//-------------------------------------------------------------------------------------------//
void RegisterEnumeratorValue::Enumerator()
{
   if(m_cValues)
   {
      clear();
      //
      for (DWORD i=0, m_lSuccess = ERROR_SUCCESS; i < m_cValues; i++) 
      { 
         m_cchValue = MAX_VALUE_NAME; 
         m_achValue = std::wstring();
         m_achValue.reserve(m_cchValue);
         //
         if((m_lSuccess = RegEnumValue(m_hKey, i, 
            LPWSTR(m_achValue.c_str()), 
            &m_cchValue, 
            NULL, 
            NULL,
            NULL,
            NULL)) == ERROR_SUCCESS ) 
         { 
            push_back(m_achValue.c_str());
         } 
      }      
   }
}
