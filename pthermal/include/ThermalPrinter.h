/*
*  Thermal API c++
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 3 2016
*  Last updated
*  Version 0.1-beta
*/
#pragma once
#pragma warning(disable:4275)
#pragma warning(disable:4251)

#include <pthermal.hpp>
#include <IGenerateTicketData.h>
#include <IThermalPrinterDefs.hpp>
#include <printcoupon.h>

namespace pthermal
{
   class PTHERMAL_API ThermalPrinter : public IThermalPrinterDefs
   {
   protected:
      LPIUniversalSerialBusData m_pHandle;
      LPXMLCFeData m_pXMLData;
      std::string m_szQuickResponseCodeFileName;
      LPITicketDataFieldQueue m_pTicketDataFieldQueue;
   public:
      ThermalPrinter ( );
      virtual ~ThermalPrinter ( );
      virtual std::wstring const  BuildQuickResponseCode ( std::wstring const & szData,
                                                           int nVersion = 1U,
                                                           unsigned uLevel = 3U,
                                                           unsigned uEncode = 2U,
                                                           unsigned uPrescaler = 8U );
      virtual bool WINAPI SearchingPrinter ( );
      virtual bool WINAPI ProcessXMLFile ( std::wstring const & wszXMLFileName );
      virtual bool ProcessTicketData ( );
      virtual bool WINAPI IsReady ( );
      virtual bool WINAPI Write ( );
      virtual bool WINAPI Build ( std::wstring const & szXMLFileName,
                                  DWORD dwDriverID = pthermal::printer::mode::WindowsDriver );
      virtual bool WINAPI Build ( std::wstring const & szXMLFileName,
                                  DWORD dwDriverID,
                                  pthermal::CouponType const & uType);
   };
   //
   typedef std::shared_ptr<ThermalPrinter> LPThermalPrinter;
}