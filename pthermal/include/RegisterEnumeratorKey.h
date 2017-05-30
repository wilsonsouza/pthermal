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

#include <IRegisterEnumeratorKey.h>

namespace reg
{
   class PTHERMAL_API RegisterEnumeratorKey: public IRegisterEnumeratorKey
   {
   public:
      explicit RegisterEnumeratorKey(HKEY hKey);
      explicit RegisterEnumeratorKey(IRegisterEnumeratorKey const & in);
      virtual ~RegisterEnumeratorKey();
      virtual void SetCurrentKey(std::wstring const & szNewKey);
      virtual std::wstring const & GetCurrentKey();
      virtual void Filter();
      virtual void Enumerator(std::wstring const & szKey = std::wstring());
   };
};