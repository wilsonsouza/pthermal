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
#include <memory>
#include <list>
#include <string>
#include <unordered_map>

namespace reg
{
   typedef std::unordered_map<std::wstring, std::list<std::wstring>> DebugMapper;
   //
   class IHKeyLocalMachine;
   struct HKeyLocalMachineSuccess;
   class IRegisterEnumeratorKey;
   class PTHERMAL_API IEnumeratorUniversalSerialBus
   {
   protected:
      std::shared_ptr<IHKeyLocalMachine> m_pLocalMachine;
      std::shared_ptr<std::list<std::list<std::wstring>>> m_pDeviceQueue;
      std::shared_ptr<DebugMapper> m_pKeyNames;
   public:
      IEnumeratorUniversalSerialBus(){}
      virtual ~IEnumeratorUniversalSerialBus(){}
      virtual std::shared_ptr<std::list<std::list<std::wstring>>> GetDeviceParameters() = 0;
      virtual std::shared_ptr<IRegisterEnumeratorKey> First() = 0;
      virtual std::shared_ptr<IRegisterEnumeratorKey> Next(
         std::shared_ptr<IRegisterEnumeratorKey> & q, std::wstring const & szNewKey = std::wstring()) = 0;
      virtual std::shared_ptr<DebugMapper> GetKeyName() = 0;
      virtual void Debug() = 0;
   };
};