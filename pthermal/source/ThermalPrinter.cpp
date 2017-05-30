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
#include <ThermalPrinter.h>
#include <UniversalSerialBusDetails.h>
#include <ExtensibleMarkupLanguage.hpp>
#include <IWcharToChar.h>
#include <Bematech.h>
#include <Daruma.h>
#include <Epson.h>
#include <SerialBus.h>
#include <QuickResponseCodeGenerator.h>
#include <algorithm>
#include <GenerateTicketData.hpp>
#include <IDeviceContext.h>
#include <IDeviceContextEx.hpp>
#include <GenerateTicketDataEx.hpp>
#include <javascriptobjectnotation.hpp>

using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
ThermalPrinter::ThermalPrinter ()
{
   this->m_pHandle = NULL;
   this->m_pXMLData = NULL;
   this->m_szQuickResponseCodeFileName = std::string ( );
   this->m_pTicketDataFieldQueue = NULL;
   IThermalPrinterDefs::SetDriverMode ( printer::mode::WindowsDriver );
}
//-----------------------------------------------------------------------------------------------//
ThermalPrinter::~ThermalPrinter ( )
{

}
//-----------------------------------------------------------------------------------------------//
bool WINAPI ThermalPrinter::SearchingPrinter ( )
{
   auto pHandle = usb::LPUniversalSerialBusDetails ( new usb::UniversalSerialBusDetails ( ) );

   pHandle->LoadParameters ( );
   //if xps not detected printer is online
   if ( ( IThermalPrinterDefs::GetDriverMode ( ) & pthermal::printer::mode::UseXPSPrinter ) == 0 )
   {
      auto p = pHandle->SearchKeyThermalPrinter ( );

      if ( p->DeviceDesc.size ( ) > 0 )
      {
         m_pHandle = p;
      }
      else
      {
         throw std::exception ( "Printer is not ready or is not connected!" );
      }
   }
   else
   {
      //assign partial handle to process xps printer 
      m_pHandle = pHandle.operator*( )[L"3xps"];
      m_pHandle->m_userial.ulPID = pthermal::xps_windows_printer::ID.ulPID;
      m_pHandle->m_userial.ulVID = pthermal::xps_windows_printer::ID.ulVID;
      m_pHandle->ThermalPrinterID = pthermal::printer::code::GetPrinterModel ( &m_pHandle->m_userial );
   }
   return m_pHandle != NULL;
}
//-----------------------------------------------------------------------------------------------//
bool WINAPI ThermalPrinter::ProcessXMLFile ( std::wstring const & wszXMLFileName )
{
   CoInitialize ( NULL );
   //
   auto pXML = xml::LPExtensibleMarkupLanguage (
      new xml::ExtensibleMarkupLanguage ( wszXMLFileName ) );
   pXML->Load ( );
   pXML->Parse ( );
   //
   m_pXMLData = pXML;
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool ThermalPrinter::ProcessTicketData ( )
{
   LPGenerateTicketData p = LPGenerateTicketData ( new GenerateTicketData ( m_pXMLData, m_pHandle ) );
   //
   m_pTicketDataFieldQueue = p->Build ( );
   //
   if ( m_pHandle )
   {
      m_pHandle->DataFieldQueue = m_pTicketDataFieldQueue;
   }
   //find qrcode
   for ( auto q = m_pTicketDataFieldQueue->begin ( ); q != m_pTicketDataFieldQueue->end ( ); q++ )
   {
      if ( q->operator->()->ulFlags & DataType::QuickResponseCode )
      {
         for ( auto d = q->operator->( )->Data->begin ( ); d != q->operator->( )->Data->end ( ); d++ )
         {
            if ( d->first.find ( m_pXMLData->m_infcfe.Id ) != -1 && !m_pXMLData->m_infcfe.Id.empty ( ) )
            {
               std::wstring szCode = d->first.c_str();
               m_pXMLData->szQuickResponseCodeFile = BuildQuickResponseCode ( szCode );
               return true;
            }
         }
      }
   }
   //
   return false;
}
//-----------------------------------------------------------------------------------------------//
bool WINAPI ThermalPrinter::Write ( )
{
   auto pHandle = m_pHandle;
   auto pQueue = m_pTicketDataFieldQueue;
   auto dwDriverMode = this->GetDriverMode ( );
   //
   if ( dwDriverMode & pthermal::printer::mode::VirtualUniversalSerialBus )
   {
      if ( pthermal::bematech_mp400_th::ID.ulPID == pHandle->m_userial.ulPID &&
           pthermal::bematech_mp400_th::ID.ulVID == pHandle->m_userial.ulVID )
      {
         auto p = bematech::LPBematech ( new bematech::Bematech ( pHandle ) );
         //
         if ( p->Open ( ) )
         {
            //
            for ( auto i = pQueue->begin ( ); i != pQueue->end ( ); i++ )
            {
               auto s = ( *i );

               if ( s->ulFlags & DataType::QuickResponseCode )
               {
                  //send qrcode data
                  p->PrintQuickResponseCode ( s );
               }
               else
               {
                  p->SendEscposCommand ( s );
               }
            }
         }
      }
      //
      else if ( pthermal::epson_tm20::ID.ulPID == pHandle->m_userial.ulPID &&
                pthermal::epson_tm20::ID.ulVID == pHandle->m_userial.ulVID )
      {
         auto p = epson::LPEpson ( new epson::Epson ( pHandle ) );
         //
         if ( p->Open ( 0 ) == S_OK )
         {
            for ( auto i = pQueue->begin ( ); i != pQueue->end ( ); i++ )
            {
               auto s = ( *i );
               //
               if ( s->ulFlags & DataType::QuickResponseCode )
               {
                  p->PrintQuickResponseCode ( s );
               }
               else
               {
                  p->SendEscposCommand ( s );
               }
            }
         }
      }
      //
      else if ( pthermal::daruma_dr800::ID.ulPID == pHandle->m_userial.ulPID &&
                pthermal::daruma_dr800::ID.ulVID == pHandle->m_userial.ulVID )
      {
         auto p = std::shared_ptr<daruma::Daruma> ( new daruma::Daruma ( pHandle ) );
         //
         if ( p->Open ( ) == S_OK )
         {
            for ( auto i = pQueue->begin ( ); i != pQueue->end ( ); i++ )
            {
               auto s = ( *i );
               //
               if ( s->ulFlags & DataType::QuickResponseCode )
               {
                  p->PrintQuickResponseCode ( s );
               }
               else
               {
                  p->SendEscposCommand ( s );
               }
            }
         }
      }
      else
      {
         throw std::exception ( "Printer is not ready or is not connected!" );
      }
   }
   else if ( dwDriverMode & ( pthermal::printer::mode::WindowsDriver |
                              pthermal::printer::mode::WindowsSpool ) )
   {
      auto p = device::LPIDeviceContext ( new device::IDeviceContext ( pHandle ) );
      //
      if ( dwDriverMode & pthermal::printer::mode::UseXPSPrinter )
      {
         //use xps printer
         p->SetPrinterMode ( pthermal::printer::mode::output::XPS );
         //p->SetDriverName ( L"WINSPOOL" );
      }
      //
      if ( dwDriverMode & pthermal::printer::mode::WindowsSpool )
      {
         //use universal driver
         //p->SetDriverName ( L"WINSPOOL" );
      }
      //
      p->Create ( m_pXMLData );
      p->PreparePage ( );
      p->Print ( );
   }
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool WINAPI ThermalPrinter::IsReady ( )
{
   auto p = usb::LPUniversalSerialBusDetails ( new usb::UniversalSerialBusDetails ( ) );
   return p->IsOnLine ( m_pHandle->m_device.PortName, m_pHandle ) == S_OK;
}
//-----------------------------------------------------------------------------------------------//
std::wstring const ThermalPrinter::BuildQuickResponseCode ( std::wstring const & szData,
                                                            int nVersion,
                                                            unsigned uLevel,
                                                            unsigned uEncode,
                                                            unsigned uPrescaler )
{
   auto p = qr::LPQuickResponseCodeGenerator ( new qr::QuickResponseCodeGenerator );
   //
   p->Build ( szData, nVersion, uLevel, uEncode, uPrescaler );
   return p->GetFileName ( );
}
//-----------------------------------------------------------------------------------------------//
bool WINAPI ThermalPrinter::Build ( std::wstring const & szXMLFileName, DWORD dwDriver )
{
   IThermalPrinterDefs::SetDriverMode ( dwDriver );
   this->SearchingPrinter ( );
   this->ProcessXMLFile ( szXMLFileName );
   this->ProcessTicketData ( );
   this->Write ( );
   return true;
}
//-----------------------------------------------------------------------------------------------//
bool WINAPI ThermalPrinter::Build ( std::wstring const & szXMLFileName,
                                    DWORD dwDriverID,
                                    pthermal::CouponType const & uType )
{
   IThermalPrinterDefs::SetDriverMode ( dwDriverID );
   this->SearchingPrinter ( );
   auto pjson = std::shared_ptr<json::JavascriptObjectNotation> ( new json::JavascriptObjectNotation ( szXMLFileName ) );
   //
   pjson->Process ( );
   pjson->utype = pthermal::TicketType ( uType );
   //
   auto dwDriverMode = IThermalPrinterDefs::GetDriverMode ( );

   if ( dwDriverMode & ( pthermal::printer::mode::WindowsDriver |
      pthermal::printer::mode::WindowsSpool ) )
   {
      auto p = device::LPIDeviceContextEx ( new device::IDeviceContextEx ( m_pHandle ) );
      //
      if ( dwDriverMode & pthermal::printer::mode::UseXPSPrinter )
      {
         //use xps printer
         p->SetPrinterMode ( pthermal::printer::mode::output::XPS );
         p->SetDriverName ( L"WINSPOOL" );
      }
      //
      if ( dwDriverMode & pthermal::printer::mode::WindowsSpool )
      {
         //use universal driver
         p->SetDriverName ( L"WINSPOOL" );
      }
      //
      p->Create ( pthermal::LPJsonData( pjson ) );
      p->PreparePage ( );
      p->Print ( );
   }
   return true;
}
//-----------------------------------------------------------------------------------------------//
