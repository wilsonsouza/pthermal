/*
*  Layer to Thermal Printer via WinSpool
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 15 2016
*  Last updated
*  August 11 2016
*  Version 0.1-beta
*/
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)

#include <pthermal.hpp>

namespace device
{
   class PTHERMAL_API IDeviceContext
   {
   protected:
      pthermal::LPIUniversalSerialBusData m_pBus;
      pthermal::LPITicketData m_Data;
      pthermal::LPXMLCFeData m_cfe;
      pthermal::LPIThermalPrinterNames m_thermal;
      pthermal::LPITicketDataFieldQueue m_queue;
      BITMAPINFO m_bmpinfo;
      BITMAP m_bmp;
      HBITMAP m_hbmp;
      std::wstring m_szDriver;
      std::wstring m_szPrinterMode;
      DOCINFO m_DocInfo;
   public:
      IDeviceContext ( pthermal::LPIUniversalSerialBusData const & pBus );
      virtual ~IDeviceContext ( );
      //
      virtual bool Create (pthermal::LPXMLCFeData const & pcfe );
      virtual bool PreparePage ();
      virtual bool Print ();
      virtual void Close ( );
      virtual void SetDriverName(std::wstring const & szDriver );
      virtual void SetPrinterMode(std::wstring const & szMode );
   protected:
      void BuildBitmap();
      void PrintQuickResponseCodeBitmap(pthermal::FieldData const & p);
      void Update();
      void GetNextXPSDocumentName ( std::wstring & szName );
   };
   //
   typedef std::shared_ptr<IDeviceContext> LPIDeviceContext;
}