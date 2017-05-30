/*
*  Thermal Printer GUI Application
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 26 2016
*  Last updated
*  Version 0.1-beta
*/
#pragma once
#include <std.application.hpp>

namespace pthermal
{
   class PThermalApp : public std::Application
   {
      Q_OBJECT
   public:
      PThermalApp ( int & argc, char ** argv );
      virtual int __fastcall Execute ( );
   };
   typedef std::shared_ptr<PThermalApp> LPPThermalApp;
}
