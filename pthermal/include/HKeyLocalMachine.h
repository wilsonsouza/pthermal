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

#include <IHKeyLocalMachine.h>

namespace reg
{
   class PTHERMAL_API HKeyLocalMachine: public IHKeyLocalMachine
   {
   public:
      explicit HKeyLocalMachine(std::wstring const & szKeyName);
      virtual ~HKeyLocalMachine();
      virtual void Access(DWORD dwAccessFlags = KEY_READ|KEY_WOW64_32KEY);
   };
};