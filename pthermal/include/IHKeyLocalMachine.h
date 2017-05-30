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

#include <HKeyLocalMachineSuccess.h>
#include <memory>

namespace reg
{
   class PTHERMAL_API IHKeyLocalMachine: public HKeyLocalMachineSuccess
   {
   protected:
      std::wstring m_szKeyName;
   public:
      IHKeyLocalMachine(){}
      explicit IHKeyLocalMachine(std::wstring const & szKeyName){}
      virtual ~IHKeyLocalMachine(){}
      virtual void Access(DWORD dwAccessFlags = KEY_READ|KEY_WOW64_32KEY) = 0;
   };
   //--------------------------------------------------------------------------------------------//
};