/*
*  Layer to USB Control
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 1 2016
*  Last updated
*  Version 0.1-beta
*/
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#include <string>
#include <EnumeratorUniversalSerialBus.h>
#include <map>

namespace usb
{
   //--------------------------------------------------------------------------------------------//
   static std::wstring copy( __in std::wstring const & in, __in wchar_t ch )
   {
      std::wstring out = std::wstring( );
      //
      for ( auto s = in.begin( ); s != in.end( ); s++ )
      {
         if ( s.operator*( ) != ch )
            out += s.operator*( );
         else
            break;
      }
      return out;
   }
   //--------------------------------------------------------------------------------------------//
   typedef std::map<std::wstring, std::shared_ptr<pthermal::IUniversalSerialBusData>> UniversalSerialBusQueue;
   class PTHERMAL_API IUniversalSerialBusDetails: 
      public reg::EnumeratorUniversalSerialBus, public UniversalSerialBusQueue
   {
   public:
      IUniversalSerialBusDetails(){}
      virtual ~IUniversalSerialBusDetails(){}
      //
      virtual void LoadParameters() = 0;
      virtual pthermal::LPIUniversalSerialBusData SearchKeyThermalPrinter() = 0;
   };
};
