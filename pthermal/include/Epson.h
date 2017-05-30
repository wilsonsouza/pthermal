/*
*  Layer to Epson Printer
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

namespace epson
{
	class PTHERMAL_API Epson: public device::IPrinterDevice
	{
	public:
		Epson ( pthermal::LPIUniversalSerialBusData const & pBus );
		virtual ~Epson ( );
		//
		virtual long Open ( bool bOverlapped = true );
		virtual bool SendEscposCommand ( pthermal::LPITicketDataField const & pCommand );
		virtual unsigned char GetStatus ( );
		virtual long Close ( );
		virtual void Reset ( );
		virtual long PrintBitmapFile ( std::wstring const & szBitmapName );
		virtual long PrintQuickResponseCode ( pthermal::LPITicketDataField const & pQuickCode,
											  unsigned uCorrectionErrorLevel = 0,
											  unsigned uWidthModuleSize = 0,
											  unsigned uCodeType = 0,
											  unsigned uQuickResponseCodeVersion = 0,
											  unsigned uEncodingMode = 0 );
	};
	typedef std::shared_ptr<Epson> LPEpson;
}