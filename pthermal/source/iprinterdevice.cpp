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
#include <IPrinterDevice.h>
#include <IWcharToChar.h>

using namespace device;
//-----------------------------------------------------------------------------------------------//
bool IPrinterDevice::SendEscposCommand ( pthermal::LPITicketDataField const & pCmd )
{
   bool bIsOnline = false;
   //
   if ( pthermal::IsVendor ( this->m_bus, &pthermal::daruma_dr800::ID ) )
   {
      bIsOnline = m_device->GetCTS ( );
   }
   else
   {
      bIsOnline = m_device->GetDSR ( );
   }
   //
   if ( bIsOnline )
   {
      for ( auto d = pCmd->Data->begin ( ); d != pCmd->Data->end ( ); d++ )
      {
         auto cbData = std::WideByte ( d->first );
         auto cbCmd = d->second;
         //send first commands
         if ( m_device->Write ( cbCmd.c_str ( ), cbCmd.length ( ) ) != S_OK )
         {
            throw std::exception ( "Unable to write COM-port!" );
         }
         //
         Sleep ( m_dwMilleseconds );
         //
         if ( m_device->Write ( cbData.c_str ( ), cbData.length ( ) ) != S_OK )
         {
            throw std::exception ( "Unable to write COM-port!" );
         }
         //wait any milleseconds to printer process data
         Sleep ( m_dwMilleseconds );
      }
   }
   else
   {
      throw std::exception ( "Printer is offline or not is connected!" );
   }
   return true;
}