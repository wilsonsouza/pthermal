#include "Daruma.h"

/*
*  Layer to Daruma Printer
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 12 2016
*  Last updated
*  Version 0.1-beta
*/
#include <Daruma.h>
#include <SerialBus.h>
#include <IWcharToChar.h>
#include <IGenerateTicketData.h>
#include <escpos.hpp>
using namespace daruma;
using namespace pthermal;
using namespace usbcom;
//-----------------------------------------------------------------------------------------------//
Daruma::Daruma ( std::shared_ptr<pthermal::IUniversalSerialBusData> const & pData ):
   device::IPrinterDevice ( pData )
{
}
//-----------------------------------------------------------------------------------------------//
Daruma::~Daruma ( )
{
   Close ( );
}
//-----------------------------------------------------------------------------------------------//
long Daruma::Open ( bool bOverlapped )
{
   long lSuccess = m_device->Open ( m_bus->m_device.PortName.c_str ( ), 0, 0, bOverlapped );
   //
   if ( lSuccess == S_OK )
   {
      lSuccess = m_device->Setup ( ISerialBus::EBaud9600,
                                   ISerialBus::EData8,
                                   ISerialBus::EParNone,
                                   ISerialBus::EStop1 );
      //
      if ( lSuccess != S_OK )
      {
         throw std::exception ( "Unable to set COM-port setting!" );
      }
      //defined doc
      DCB dcb;
      //
      if ( !GetCommState ( m_device->GetCommHandle ( ), &dcb ) )
      {
         std::exception ( "Unable to get comm state!" );
      }
      //specified for daruma
      dcb.fOutxCtsFlow = true;
      dcb.fOutxDsrFlow = false;
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
      dcb.fOutX = false;
      dcb.fInX = false;
      //
      if ( !SetCommState ( m_device->GetCommHandle ( ), &dcb ) )
      {
         throw std::exception ( "Unable to set COM-port handshaking!" );
      }
   }
   else
   {
      throw std::exception ( "Unable to open COM-port!" );
   }
   return lSuccess == S_OK;
}
//-----------------------------------------------------------------------------------------------//
bool Daruma::SendEscposCommand ( pthermal::LPITicketDataField const & pCommand )
{
   return IPrinterDevice::SendEscposCommand ( pCommand );
}
//-----------------------------------------------------------------------------------------------//
unsigned char Daruma::GetStatus ( )
{
   bool bSuccess = m_device->GetCTS ( );
   BYTE cbCmd [ ] = { 0x05 };
   //
   if ( !bSuccess )
   {
      throw std::exception ( "Printer is offine or not is connected!" );
   }
   //
   if ( m_device->Write ( cbCmd, sizeof ( cbCmd ) ) != S_OK )
   {
      throw std::exception ( "Unable to write COM-port!" );
   }
   //
   Sleep ( m_dwMilleseconds );
   unsigned char ucBits;
   long lSuccess = m_device->Read ( &ucBits, sizeof ( ucBits ) );
   //
   if ( lSuccess != S_OK )
   {
      throw std::exception ( "Unable to read COM-port!" );
   }
   //
   return ucBits;
}
//-----------------------------------------------------------------------------------------------//
long Daruma::Close ( )
{
   return m_device->Close ( );
}
//-----------------------------------------------------------------------------------------------//
void Daruma::Reset ( )
{
   bool bSuccess = m_device->GetCTS ( );
   BYTE cbCmd [ ] = { 0x1b, 0x40 };
   //
   if ( !bSuccess )
   {
      throw std::exception ( "Printer is offilne or not is connected!" );
   }
   //
   if ( m_device->Write ( cbCmd, sizeof ( cbCmd ) ) != S_OK )
   {
      throw std::exception ( "Unable to write COM-port!" );
   }
}
//-----------------------------------------------------------------------------------------------//
long Daruma::PrintBitmapFile ( std::wstring const & szBitmapName )
{
   return 0;
}
//-----------------------------------------------------------------------------------------------//
long Daruma::PrintQuickResponseCode ( pthermal::LPITicketDataField const & pQuickCode,
                                      unsigned uCorrectionErrorLevel,
                                      unsigned uWidthModuleSize,
                                      unsigned uCodeType,
                                      unsigned uQuickResponseCodeVersion,
                                      unsigned uEncodingMode )
{
   pthermal::LPITicketDataField p = pQuickCode;
   auto szCode = p->Data->operator[]( 2 );
   size_t uLength = szCode.first.length ( );
   size_t uMinus = LOBYTE ( uLength );
   size_t uPlus = HIBYTE ( uLength ) + 2;
   //
   BYTE cbQR [ ] =
   {
      0x1b,
      0x81,
      BYTE ( uMinus ),
      BYTE ( uPlus ),
      BYTE ( uWidthModuleSize ),
      BYTE ( uCorrectionErrorLevel )
   };
   BYTE cbLastBytes [ ] = { BYTE ( uLength - 2 ) };
   //
   return SendEscposCommand ( p );
}
//-----------------------------------------------------------------------------------------------//
