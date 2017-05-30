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
#include <HKeyLocalMachineSuccess.h>
#include <wchar.h>
using namespace reg;
//-------------------------------------------------------------------------------------------//
RegisterEnumeratorKey::RegisterEnumeratorKey(HKEY hKey):IRegisterEnumeratorKey(hKey)
{
   this->m_hKey = hKey;
   this->m_cchClassName = MAX_PATH;
   this->m_cSubKeys = 0;
   this->m_cchValue = MAX_VALUE_NAME;
   this->m_achKey.reserve(MAX_KEY_LENGTH);
   this->m_achClass.reserve(MAX_PATH);
   this->m_achValue.reserve(MAX_VALUE_NAME);
}
//-------------------------------------------------------------------------------------------//
RegisterEnumeratorKey::RegisterEnumeratorKey(IRegisterEnumeratorKey const & inp):
IRegisterEnumeratorKey(inp)
{
   RegisterEnumeratorKey & in = 
      reinterpret_cast<RegisterEnumeratorKey &>(const_cast<IRegisterEnumeratorKey &>(inp));
   //
   //this->m_achClass = in.m_achClass;
   //this->m_achKey   = in.m_achKey;
   //this->m_achValue = in.m_achValue;
   this->m_cbMaxSubKey = in.m_cbMaxSubKey;
   this->m_cbMaxValueData = in.m_cbMaxSubKey;
   this->m_cbName = in.m_cbName;
   this->m_cbSecurityDescriptor = in.m_cbSecurityDescriptor;
   this->m_cchClassName = in.m_cchClassName;
   this->m_cchMaxClass = in.m_cchMaxClass;
   this->m_cchMaxValue = in.m_cchMaxValue;
   this->m_cchValue = in.m_cchValue;
   this->m_cSubKeys = in.m_cSubKeys;
   this->m_cValues = in.m_cValues;
   this->m_ftLastWriteTime = in.m_ftLastWriteTime;
   this->m_hKey = in.m_hKey;
   this->m_lSuccess = in.m_lSuccess;
   this->m_szCurrentKey = in.m_szCurrentKey;
   //
   clear();
}
//-------------------------------------------------------------------------------------------//
RegisterEnumeratorKey::~RegisterEnumeratorKey()
{
   clear();
}
//-------------------------------------------------------------------------------------------//
void RegisterEnumeratorKey::SetCurrentKey(std::wstring const & szNewKey)
{
   this->m_szCurrentKey = szNewKey;
}
//-------------------------------------------------------------------------------------------//
std::wstring const & RegisterEnumeratorKey::GetCurrentKey()
{
   return this->m_szCurrentKey;
}
//-------------------------------------------------------------------------------------------//
void RegisterEnumeratorKey::Filter()
{
   m_achClass = std::wstring();
   m_achKey.reserve(MAX_KEY_LENGTH);
   // Get the class name and the value count. 
   m_lSuccess = RegQueryInfoKey(
      m_hKey,                    // key handle 
      LPWSTR(m_achClass.c_str()),// buffer for class name 
      &m_cchClassName,           // size of class string 
      NULL,                      // reserved 
      &m_cSubKeys,               // number of subkeys 
      &m_cbMaxSubKey,            // longest subkey size 
      &m_cchMaxClass,            // longest class string 
      &m_cValues,                // number of values for this key 
      &m_cchMaxValue,            // longest value name 
      &m_cbMaxValueData,         // longest value data 
      &m_cbSecurityDescriptor,   // security descriptor 
      &m_ftLastWriteTime);       // last write time 
   //
   if(m_lSuccess != ERROR_SUCCESS)
   {
      throw std::exception(reg::ErrorMsg(__FUNCTION__, __LINE__, "Key not found!")->c_str());
   }
}
//-------------------------------------------------------------------------------------------//
void RegisterEnumeratorKey::Enumerator(std::wstring const & szKey)
{
   //clear list
   clear();
   //
   if(m_cSubKeys)
   {
      for (DWORD i=0; i< m_cSubKeys; i++) 
      { 
         m_cbName = MAX_KEY_LENGTH;
         m_achKey = std::wstring();
         m_achKey.reserve(MAX_KEY_LENGTH);
         //
         if((m_lSuccess = RegEnumKeyEx(m_hKey, 
            i, 
            LPWSTR(m_achKey.c_str()), 
            &m_cbName, 
            NULL, 
            NULL, 
            NULL, 
            &m_ftLastWriteTime)) == ERROR_SUCCESS)
         {
            std::wstring comp = m_achKey.c_str();
            //check key 
            if(szKey.size() == 0)
            {
               push_back(m_achKey.c_str());
            }
            else if(szKey.size() > 0)
            {
               bool bSuccess = (std::wcsstr(pthermal::KEY_0003.c_str(), comp.c_str()) ||
                  std::wcsstr(pthermal::KEY_0005.c_str(), comp.c_str()) ||
                  std::wcsstr(pthermal::KEY_0202.c_str(), comp.c_str()));
               //
               if(bSuccess)
               {
                  push_back(m_achKey.c_str());
               }
            }
         }
         else
         {
            throw std::exception(reg::ErrorMsg(__FUNCTION__, __LINE__, "Key not found!")->c_str());
         }
      }
   }
}
