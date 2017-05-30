/*
*  Layer to Daruma Printer
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
#include <IPrinterDevice.h>

namespace daruma
{
   class PTHERMAL_API Daruma : public device::IPrinterDevice
   {
   public:
      explicit Daruma ( std::shared_ptr<pthermal::IUniversalSerialBusData> const & pData );
      virtual ~Daruma ( );
      //
      virtual long Open ( bool bOverlapped = true );
      virtual bool SendEscposCommand ( pthermal::LPITicketDataField const & pCommand );
      virtual unsigned char GetStatus ( );
      virtual long Close ( );
      virtual void Reset ( );
      virtual long PrintBitmapFile ( std::wstring const & szBitmapName );
      virtual long PrintQuickResponseCode ( pthermal::LPITicketDataField const & pQuickCode,
                                            unsigned uCorrectionErrorLevel = 0x4d,
                                            unsigned uWidthModuleSize = 0x4,
                                            unsigned uCodeType = 0,
                                            unsigned uQuickResponseCodeVersion = 0,
                                            unsigned uEncodingMode = 0 );
   };
   //
   typedef std::shared_ptr<Daruma> LPDaruma;
};