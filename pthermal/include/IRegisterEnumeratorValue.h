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
   class PTHERMAL_API IRegisterEnumeratorValue
   {
   public:
      IRegisterEnumeratorValue(){}
      IRegisterEnumeratorValue(IRegisterEnumeratorKey const & in){}
      virtual ~IRegisterEnumeratorValue(){}
      virtual void Enumerator() = 0;
   };
};