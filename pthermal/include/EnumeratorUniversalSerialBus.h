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
#include <IEnumeratorUniversalSerialBus.h>

namespace reg
{
   class PTHERMAL_API EnumeratorUniversalSerialBus: public IEnumeratorUniversalSerialBus
   {
   public:
      EnumeratorUniversalSerialBus();
      virtual ~EnumeratorUniversalSerialBus();
      virtual std::shared_ptr<std::list<std::list<std::wstring>>> GetDeviceParameters();
      virtual std::shared_ptr<IRegisterEnumeratorKey> First();
      virtual std::shared_ptr<IRegisterEnumeratorKey> Next(
         std::shared_ptr<IRegisterEnumeratorKey> & q, std::wstring const & szNewKey = std::wstring());
      virtual std::shared_ptr<DebugMapper> GetKeyName();
      virtual void Debug();
   };
};