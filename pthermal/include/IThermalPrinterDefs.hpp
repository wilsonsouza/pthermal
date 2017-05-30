/*
*  Layer to Thermal Printer
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 9 2016
*  Last updated
*  Version 0.1-beta
*/
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#pragma warning(disable:4229)
#include <pthermal.hpp>

namespace pthermal
{
   struct PTHERMAL_API IThermalPrinterDefs
   {
      static unsigned long m_ulDriverMode;
      static DWORD const GetDriverMode ( );
      static void const SetDriverMode ( DWORD dwDriverMode );
   };
}