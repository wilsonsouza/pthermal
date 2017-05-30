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
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)

#include <pthermal.hpp>
#include <string>
#include <list>

namespace reg
{
   class PTHERMAL_API IRegisterEnumeratorKey: public std::list<std::wstring>
   {
   protected:
      HKEY m_hKey;
      std::wstring m_achKey;   // buffer for subkey name
      DWORD m_cbName;                   // size of name string 
      std::wstring m_achClass;  // buffer for class name 
      DWORD m_cchClassName;  // size of class string 
      DWORD m_cSubKeys;               // number of subkeys 
      DWORD m_cbMaxSubKey;              // longest subkey size
      DWORD m_cchMaxClass;              // longest class string 
      DWORD m_cValues;              // number of values for key 
      DWORD m_cchMaxValue;          // longest value name 
      DWORD m_cbMaxValueData;       // longest value data 
      DWORD m_cbSecurityDescriptor; // size of security descriptor 
      FILETIME m_ftLastWriteTime;      // last write time 
      DWORD m_lSuccess; 
      std::wstring m_achValue; 
      DWORD m_cchValue; 
      std::wstring m_szCurrentKey;
   public:
      explicit IRegisterEnumeratorKey(){}
      explicit IRegisterEnumeratorKey(HKEY hKey){}
      explicit IRegisterEnumeratorKey(IRegisterEnumeratorKey const & in){};
      virtual ~IRegisterEnumeratorKey(){}
      virtual void SetCurrentKey(std::wstring const & szNewKey) = 0;
      virtual std::wstring const & GetCurrentKey() = 0;
      virtual void Filter() = 0;
      virtual void Enumerator(std::wstring const & szKey = std::wstring()) = 0;
   };
};