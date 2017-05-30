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

#include <pthermal.hpp>
#include <SerialBus.h>

namespace device
{
	class PTHERMAL_API IPrinterDevice
	{
	protected:
		std::shared_ptr<pthermal::IUniversalSerialBusData> m_bus;
		std::shared_ptr<usbcom::SerialBus> m_device;
      DWORD m_dwMilleseconds;

	public:
		IPrinterDevice (pthermal::LPIUniversalSerialBusData const & pBus )
		{
			m_bus = pBus;
			m_device = usbcom::LPSerialBus ( new usbcom::SerialBus );
         m_dwMilleseconds = 0x32;
		}
		virtual ~IPrinterDevice ( ) { }
		//
		virtual long Open ( bool bOverlapped = true ) = 0;
		virtual bool SendEscposCommand ( pthermal::LPITicketDataField const & pCommand );
		virtual unsigned char GetStatus ( ) = 0;
		virtual long Close ( ) = 0;
		virtual void Reset ( ) = 0;
		virtual long PrintBitmapFile ( std::wstring const & szBitmapName ) = 0;
		virtual long PrintQuickResponseCode ( pthermal::LPITicketDataField const & pQuickCode,
											  unsigned uCorrectionErrorLevel = 0,
											  unsigned uWidthModuleSize = 0,
											  unsigned uCodeType = 0,
											  unsigned uQuickResponseCodeVersion = 0,
											  unsigned uEncodingMode = 0 ) = 0;
	};
}