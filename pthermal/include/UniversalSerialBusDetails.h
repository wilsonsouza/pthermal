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
#include <IUniversalSerialBusDetails.h>

namespace usb
{
   namespace printer
   {
      static unsigned int IsNotReady = 0x10;
   };
   //
   class PTHERMAL_API UniversalSerialBusDetails : public IUniversalSerialBusDetails
   {
   protected:
      std::shared_ptr<std::list<std::wstring>> m_pDeviceList;
   public:
      UniversalSerialBusDetails ( );
      virtual ~UniversalSerialBusDetails ( );
      //
      virtual void LoadParameters ( );
      virtual pthermal::LPIUniversalSerialBusData SearchKeyThermalPrinter ( );
      virtual unsigned long IsOnLine ( __in std::wstring const & wszPortName,
                                       __in pthermal::LPIUniversalSerialBusData const & pBus );
      virtual unsigned long IsOnLine ( __in pthermal::LPIUniversalSerialBusData const & pUsb );
   protected:
      virtual void SetVIDPID ( __inout pthermal::LPIUniversalSerialBusData & pUsb,
                               __in std::wstring const & szData );
      virtual void SetValue ( __inout std::wstring & wszVar,
                              __inout std::wstring const & wszValue,
                              __in std::wstring const & wszName,
                              __in std::wstring const & wszLocal );
      bool IsPrinterConnectedByUniversalSerialBus(__in pthermal::LPIUniversalSerialBusData const & pBus );
   };
   //
   typedef std::shared_ptr<UniversalSerialBusDetails> LPUniversalSerialBusDetails;
};
