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
#pragma warning(disable:4200)
#include <libusb.h>
#include <UniversalSerialBusDetails.h>
#include <EnumeratorUniversalSerialBus.h>
#include <HKeyLocalMachine.h>
#include <algorithm>
#include <SerialBus.h>
using namespace usb;
using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
UniversalSerialBusDetails::UniversalSerialBusDetails ( ):IUniversalSerialBusDetails ( )
{
   this->GetDeviceParameters ( );
}
//-----------------------------------------------------------------------------------------------//
UniversalSerialBusDetails::~UniversalSerialBusDetails ( )
{
}
//-----------------------------------------------------------------------------------------------//
void UniversalSerialBusDetails::LoadParameters ( )
{
   auto pQueue = this->m_pKeyNames;
   //
   clear ( );
   //
   operator[]( L"1bematech" ) = LPIUniversalSerialBusData ( new IUniversalSerialBusData ( ) );
   operator[]( L"2epson" ) = LPIUniversalSerialBusData ( new IUniversalSerialBusData ( ) );
   operator[]( L"3daruma" ) = LPIUniversalSerialBusData ( new IUniversalSerialBusData ( ) );
   operator[]( L"3xps" ) = LPIUniversalSerialBusData ( new IUniversalSerialBusData ( ) );

   //fill  printer id
   operator[]( L"1bematech" )->m_userial.ulPrinterID = pthermal::printer::code::id::Bematech_MP400_TH;
   operator[]( L"2epson" )->m_userial.ulPrinterID = pthermal::printer::code::id::Epson_TM20;
   operator[]( L"3daruma" )->m_userial.ulPrinterID = pthermal::printer::code::id::Daruma_DR800;
   operator[]( L"3xps" )->m_userial.ulPrinterID = pthermal::printer::code::id::XPSPrinter;

   //remove keys which not used
   for ( auto q = pQueue->begin ( ); q != pQueue->end ( ); q++ )
   {
      if ( q->first.find ( L"Ceip" ) != std::wstring::npos ||
           q->first.find ( L"Properties" ) != std::wstring::npos )
      {
         pQueue->erase ( q );
         q = pQueue->begin ( );
      }
   }
   //get subkey
   for ( auto p = pQueue->begin ( ); p != pQueue->end ( ); p++ )
   {
      auto wszKey = p->first;
      auto q = p->second;
      LPIUniversalSerialBusData pUsb = NULL;
      std::basic_string<WCHAR> wszMapKey;
      int n = 0;
      //
      for ( auto j = begin ( ); j != end ( ); j++, n++ )
      {
         auto p0 = LPIUniversalSerialBusData ( new IUniversalSerialBusData ( ) );
         //
         SetVIDPID ( p0, wszKey );
         //
         if ( pthermal::IsVendor ( p0, pthermal::printer::code::g_pThermalListId[n] ) )
         {
            pUsb = j->second;
            wszMapKey = j->first;
            p0.reset ( );
            j->second.reset ( );
            break;
         }
         //
         p0.reset ( );
      }
      //set VID & PID usb access key
      SetVIDPID ( pUsb, wszKey );

      for ( auto l = q.begin ( ); l != q.end ( ); l++ )
      {
         auto wszValue = *l;
         auto acData = std::wstring ( );
         DWORD cbData = reg::MAX_VALUE_NAME;
         auto lSuccess = 0L;
         auto dwData = 0L;
         //
         //set variable data
         acData.reserve ( cbData );
         //
         if ( wszValue == L"Capibilities" || wszValue == L"ConfigFlags" || wszValue == L"IsPrinter" )
         {
            lSuccess = RegGetValue ( HKEY_LOCAL_MACHINE,
                                     wszKey.c_str ( ),
                                     wszValue.c_str ( ),
                                     RRF_RT_DWORD,
                                     NULL,
                                     &dwData,
                                     &cbData );
         }
         else
         {
            lSuccess = RegGetValue ( HKEY_LOCAL_MACHINE,
                                     wszKey.c_str ( ),
                                     wszValue.c_str ( ),
                                     RRF_RT_ANY,
                                     NULL,
                                     PVOID ( acData.c_str ( ) ),
                                     &cbData );
         }
         //
         if ( lSuccess == ERROR_SUCCESS )
         {
            if ( wszValue == L"Capibilities" && pUsb->Capabilites == 0 )
               pUsb->Capabilites = dwData;
            else if ( wszValue == L"ConfigFlags" && pUsb->ConfigFlags == 0 )
               pUsb->ConfigFlags = dwData;
            else if ( wszValue == L"IsPrinter" && pUsb->m_device.IsPrinter == 0 )
               pUsb->m_device.IsPrinter = dwData;
            //
            SetValue ( pUsb->ClassGUID, acData, wszValue, L"ClassGUID" );
            SetValue ( pUsb->CompatibleIDs, acData, wszValue, L"CompatibleIDs" );
            SetValue ( pUsb->Service, acData, wszValue, L"Service" );
            SetValue ( pUsb->m_device.PortName, acData, wszValue, L"PortName" );
            SetValue ( pUsb->DeviceDesc, acData, wszValue, L"DeviceDesc" );
            SetValue ( pUsb->FriendlyName, acData, wszValue, L"FriendlyName" );
            SetValue ( pUsb->ContainerID, acData, wszValue, L"ContainerID" );
            SetValue ( pUsb->HardwareID, acData, wszValue, L"HardwareID" );
            SetValue ( pUsb->Mfg, acData, wszValue, L"Mfg" );
            SetValue ( pUsb->LocationInformation, acData, wszValue, L"LocaltionInformation" );
            SetValue ( pUsb->m_device.DeviceObjectName, acData, wszValue, L"DeviceObjectName" );
            SetValue ( pUsb->m_device.DriverUserInterfaceGuid, acData, wszValue, L"DriverUserInterfaceGuid" );
            SetValue ( pUsb->Driver, acData, wszValue, L"Driver" );
         }
      }
      //
      this->operator[]( wszMapKey.c_str ( ) ) = pUsb;
   }
}
//-----------------------------------------------------------------------------------------------//
pthermal::LPIUniversalSerialBusData UniversalSerialBusDetails::SearchKeyThermalPrinter ( )
{
   DWORD dwDriverMode = IThermalPrinterDefs::GetDriverMode ( );
   //
   for ( auto i = begin ( ); i != end ( ); i++ )
   {
      if ( pthermal::IsVendor ( i->second, &pthermal::epson_tm20::ID ) )
      {
         auto pwszName = std::wcsstr ( i->second->DeviceDesc.c_str ( ), L";" );
         auto pwszNewDesc = std::wstring ( pwszName + 1 );
         //
         i->second->m_device.PortName = L"COM2";
         i->second->m_device.VPortName = L"ESDPRT001";
         i->second->DeviceDesc = pwszNewDesc.c_str ( );
         i->second->FriendlyName = i->second->DeviceDesc.c_str ( );
         this->operator[]( i->first ) = ( *i ).second;
      }
      //
      auto pThermal = pthermal::printer::code::GetPrinterModel ( &i->second->m_userial );
      //
      if ( pThermal->ThermalPrinterID )
      {
         i->second->ThermalPrinterID =
            std::make_shared<pthermal::IThermalPrinterNames> ( *pThermal );
      }
      //use usbcom
      if ( dwDriverMode & pthermal::printer::mode::VirtualUniversalSerialBus )
      {
         if ( IsOnLine ( i->second->m_device.PortName, i->second ) == S_OK )
         {
            return i->second;
         }
      }
      //use windriver or dynamiclibrary of factory
      else if ( dwDriverMode & ( pthermal::printer::mode::DynamicLibaryPrinter |
                                 pthermal::printer::mode::WindowsDriver ) )
      {
         if ( dwDriverMode & pthermal::printer::mode::WindowsDriver )
         {
            if ( IsOnLine ( i->second ) == S_OK )
            {
               return i->second;
            }
         }
         else
         {
            if ( IsOnLine ( i->second->m_device.PortName, i->second ) == S_OK )
            {
               return i->second;
            }
         }
      }
      else if(dwDriverMode & (pthermal::printer::mode::WindowsSpool ) )
      {
         if ( IsOnLine ( i->second ) == S_OK )
         {
            return i->second;
         }
      }
   }
   return std::make_shared<IUniversalSerialBusData> ( IUniversalSerialBusData ( ) );
}
//-----------------------------------------------------------------------------------------------//
unsigned long UniversalSerialBusDetails::IsOnLine ( std::wstring const & wszPortName,
                                                    pthermal::LPIUniversalSerialBusData const & pBus )
{
   auto p = usbcom::LPSerialBus ( new usbcom::SerialBus ( ) );
   DWORD dwSuccess = p->Open ( wszPortName.c_str ( ), 0, 0, false );
   std::wstring szPort = wszPortName;

   //if epson TM20 could use other port name type ESDPRT00?
   if ( pthermal::IsVendor ( pBus, &pthermal::epson_tm20::ID ) && dwSuccess != S_OK )
   {
      dwSuccess = p->Open ( pBus->m_device.VPortName.c_str ( ), 0, 0, false );
   }
   //
   if ( dwSuccess == S_OK )
   {
      if ( pthermal::IsVendor (pBus, &pthermal::daruma_dr800::ID ) )
      {
         if ( !p->GetCTS ( ) )
            dwSuccess = usb::printer::IsNotReady;
      }
      else
      {
         if ( !p->GetDSR ( ) )
            dwSuccess = usb::printer::IsNotReady;
      }
   }
   //
   return dwSuccess;
}
//-----------------------------------------------------------------------------------------------//
unsigned long usb::UniversalSerialBusDetails::IsOnLine ( pthermal::LPIUniversalSerialBusData const & pUsb )
{
   if ( pUsb->ThermalPrinterID )
   {
      if(pthermal::IThermalPrinterDefs::GetDriverMode() & pthermal::printer::mode::WindowsDriver )
      {
         bool bSuccess = IsPrinterConnectedByUniversalSerialBus ( pUsb );
         //
         if(bSuccess )
            return S_OK;
         return 1L;
      }
      //
      if ( OpenPrinter ( LPWSTR ( pUsb->ThermalPrinterID->Printer.c_str ( ) ),
                           &pUsb->ThermalPrinterID->hPrinter,
                           NULL ) )
      {
         ClosePrinter ( pUsb->ThermalPrinterID->hPrinter );
         return S_OK;
      }
   }
   return 1L;
}
//-----------------------------------------------------------------------------------------------//
void UniversalSerialBusDetails::SetVIDPID ( pthermal::LPIUniversalSerialBusData & pUsb,
                                            std::wstring const & szData )
{
   auto offset = szData.length ( );
   auto f = szData;
   auto data = std::wstring ( );
   auto key = L"VID";
   //
   for ( auto s = f.begin ( ); s != f.end ( ); s++ )
   {
      if ( *s != *std::wstring ( L"\\" ).c_str ( ) )
         data += *s;
      else
      {
         if ( data.substr ( 0, 3 ) == key )
         {
            break;
         }
         else
         {
            data = std::wstring ( );
         }
      }
   }
   //
   if ( data.size ( ) > 0 )
   {
      auto s = data;
      auto v = copy ( s, '&' );
      auto p = copy ( s.c_str ( ) + std::wcslen ( v.c_str ( ) ) + 1, '&' );
      auto vi = std::wstring ( v ).substr ( 4, v.length ( ) );
      auto pi = std::wstring ( p ).substr ( 4, p.length ( ) );
      //
      pUsb->m_userial.ulVID = std::stol ( vi, 0, 0x10 );
      pUsb->m_userial.ulPID = std::stol ( pi, 0, 0x10 );
   }
}
//-----------------------------------------------------------------------------------------------//
void UniversalSerialBusDetails::SetValue ( std::wstring & wszVar,
                                           std::wstring const & wszValue,
                                           std::wstring const & wszName,
                                           std::wstring const & wszLocal )
{
   if ( wszName == wszLocal && wszVar.size ( ) == 0 )
   {
      wszVar = wszValue.c_str ( );
   }
}
//-----------------------------------------------------------------------------------------------//
bool UniversalSerialBusDetails::IsPrinterConnectedByUniversalSerialBus ( 
__in pthermal::LPIUniversalSerialBusData const & pBus )
{
   libusb_device ** devices = NULL;
   libusb_device * p = NULL;
   struct libusb_device_descriptor dd;
   int i = 0;
   bool bSuccess = false;
   //start usb
   if ( libusb_init ( NULL ) < S_OK )
   {
      return false;
   }
   //get device list
   if ( libusb_get_device_list ( NULL, &devices ) < S_OK )
   {
      return false;
   }
   //search by specific device
   while ( ( p = devices [ i++ ] ) != NULL && !bSuccess )
   {
      if ( libusb_get_device_descriptor ( p, &dd ) < S_OK )
      {
         break;
      }
      //
      pthermal::IUniversalSerialBusData::VirtualPrinterID hVID
      { 
         dd.idVendor, dd.idProduct 
      };
      bSuccess = pthermal::IsVendor ( &hVID, &pBus->m_userial );
   }
   //free device list and close usb
   libusb_free_device_list ( devices, 1 );
   libusb_exit ( NULL );
   return bSuccess;
}
//-----------------------------------------------------------------------------------------------//
