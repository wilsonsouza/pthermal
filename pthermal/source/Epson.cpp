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
#include <Epson.h>
#include <IUniversalSerialBusDetails.h>
#include <IWcharToChar.h>
#include <QuickResponseCodeGenerator.h>
#include <escpos.hpp>
using namespace pthermal;
using namespace usbcom;
using namespace epson;
//-----------------------------------------------------------------------------------------------//
Epson::Epson ( std::shared_ptr<pthermal::IUniversalSerialBusData> const & pData ):
   device::IPrinterDevice ( pData )
{
}
//-----------------------------------------------------------------------------------------------//
Epson::~Epson ( )
{
   Close ( );
}
//-----------------------------------------------------------------------------------------------//
long Epson::Open ( bool bOverlapped )
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
      //
      if ( m_device->SetupHandshaking ( ISerialBus::EHandshakeHardware ) != S_OK )
      {
         throw std::exception ( "Unable to set hardware mode!" );
      }
      //set font
      //restart printer
      //define general configuration
      LPITicketDataField pCmd = LPITicketDataField ( new ITicketDataField );
      //
      pCmd->Data->push_back ( pthermal::FieldData(L"", escpos::PrinterEnable ) );
      pCmd->Data->push_back ( pthermal::FieldData (L"", escpos::Character::SelectCharacterFontB ) );
      //
      SendEscposCommand ( pCmd );
   }
   else
   {
      throw std::exception ( "Unable to open COM-port!" );
   }
   return lSuccess == S_OK;
}
//-----------------------------------------------------------------------------------------------//
bool Epson::SendEscposCommand ( pthermal::LPITicketDataField const & pCommand )
{
   return IPrinterDevice::SendEscposCommand ( pCommand );
}
//-----------------------------------------------------------------------------------------------//
unsigned char Epson::GetStatus ( )
{
   bool bSuccess = m_device->GetDSR ( );
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
long Epson::Close ( )
{
   return m_device->Close ( );
}
//-----------------------------------------------------------------------------------------------//
void Epson::Reset ( )
{
   bool bSuccess = m_device->GetDSR ( );
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
long Epson::PrintBitmapFile ( std::wstring const & szBitmapName )
{
   return 0;
}
//-----------------------------------------------------------------------------------------------//
long Epson::PrintQuickResponseCode ( pthermal::LPITicketDataField const & pQuickCode,
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
   return 0; //SendEscposCommand ( p );
}
//-----------------------------------------------------------------------------------------------//
