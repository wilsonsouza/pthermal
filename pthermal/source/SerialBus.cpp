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
#include <pthermal.hpp>
#include <crtdbg.h>
#include <SetupAPI.h>
#include <SerialBus.h>
//////////////////////////////////////////////////////////////////////
// Disable warning C4127: conditional expression is constant, which
// is generated when using the _RPTF and _ASSERTE macros.

#pragma warning(disable: 4127)


//////////////////////////////////////////////////////////////////////
// Enable debug memory manager
//////////////////////////////////////////////////////////////////////
// Helper methods
using namespace usbcom;

inline void SerialBus::CheckRequirements ( LPOVERLAPPED lpOverlapped, DWORD dwTimeout ) const
{
#ifdef SERIAL_NO_OVERLAPPED

   // Check if an overlapped structure has been specified
   if ( lpOverlapped || ( dwTimeout != INFINITE ) )
   {
      // Quit application
      ::MessageBox ( 0, _T ( "Overlapped I/O and time-outs are not supported, when overlapped I/O is disabled." ), _T ( "Serial library" ), MB_ICONERROR | MB_TASKMODAL );
      ::DebugBreak ( );
      ::ExitProcess ( 0xFFFFFFF );
   }

#endif

#ifdef SERIAL_NO_CANCELIO

   // Check if 0 or INFINITE time-out has been specified, because
   // the communication I/O cannot be cancelled.
   if ( ( dwTimeout != 0 ) && ( dwTimeout != INFINITE ) )
   {
      // Quit application
      ::MessageBox ( 0, _T ( "Timeouts are not supported, when SERIAL_NO_CANCELIO is defined" ), _T ( "Serial library" ), MB_ICONERROR | MB_TASKMODAL );
      ::DebugBreak ( );
      ::ExitProcess ( 0xFFFFFFF );
   }

#endif	// SERIAL_NO_CANCELIO

   // Avoid warnings
   ( void ) dwTimeout;
   ( void ) lpOverlapped;
}

inline BOOL SerialBus::CancelCommIo ( void )
{
#ifdef SERIAL_NO_CANCELIO
   // CancelIo shouldn't have been called at this point
   ::DebugBreak ( );
   return FALSE;
#else

   // Cancel the I/O request
   return ::CancelIo ( m_hFile );

#endif	// SERIAL_NO_CANCELIO
}


//////////////////////////////////////////////////////////////////////
// Code

SerialBus::SerialBus ( )
{
   m_acPortNamesFriendly = std::make_shared<std::vector<std::ustring>> ( std::vector<std::ustring> ( ) );
   m_bInited = false;
}

SerialBus::~SerialBus ( )
{
   // If the device is already closed,
   // then we don't need to do anything.
   if ( m_hFile )
   {
      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::~SerialBus - Serial port not closed\n" );

      // Close implicitly
      Close ( );
   }
}

// needed for serial bus enumeration:
// 4d36e978-e325-11ce-bfc1-08002be10318}
DEFINE_GUID ( GUID_SERENUM_BUS_ENUMERATOR, 0x4D36E978, 0xE325,
              0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 );

void SerialBus::EnumerateWin32Ports ( )
{
   HDEVINFO hDevInfo = nullptr;
   SP_DEVINFO_DATA DeviceInterfaceData;
   DWORD dwDataType = 0;
   DWORD dwActualSize = 0;
   //
   if ( m_bInited )
   {
      return;
   }

   // Search device set
   hDevInfo = SetupDiGetClassDevs ( ( struct _GUID * )&GUID_SERENUM_BUS_ENUMERATOR,
                                    0,
                                    0,
                                    DIGCF_PRESENT );
   if ( hDevInfo ) 
   {
      int i = 0;
      std::ustring acDataBuf = std::shared_ptr<BYTE> ( new BYTE [ MAX_PATH + 1 ] ).get ( );
      //
      while ( TRUE ) 
      {
         ZeroMemory ( &DeviceInterfaceData, sizeof ( DeviceInterfaceData ) );
         DeviceInterfaceData.cbSize = sizeof ( DeviceInterfaceData );

         if ( !SetupDiEnumDeviceInfo ( hDevInfo, i, &DeviceInterfaceData ) ) 
         {
            // SetupDiEnumDeviceInfo failed
            break;
         }

         if ( SetupDiGetDeviceRegistryPropertyA ( hDevInfo,
                                                  &DeviceInterfaceData,
                                                  SPDRP_FRIENDLYNAME,
                                                  &dwDataType,
                                                  LPBYTE ( acDataBuf.c_str ( ) ),
                                                  acDataBuf.length ( ),
                                                  &dwActualSize ) )
         {
            std::ustring uData = acDataBuf;
            std::ustring uPort = std::ustring ( );
            //
            for ( auto c = uData.begin ( ); c != uData.end ( ); c++ )
            {
               if ( *c == '(' || *c == ')' )
                  continue;
               else
               {
                  uPort += *c;
               }
            }
            //
            m_acPortNamesFriendly->push_back ( uPort );
         }
         i++;
      }
   }
   SetupDiDestroyDeviceInfoList ( hDevInfo );
   m_bInited = false;
}

SerialBus::EPort SerialBus::CheckPort ( LPCTSTR lpszDevice )
{
   // Try to open the device
   HANDLE hFile = ::CreateFile ( lpszDevice,
                                 GENERIC_READ | GENERIC_WRITE,
                                 0,
                                 0,
                                 OPEN_EXISTING,
                                 0,
                                 0 );

   // Check if we could open the device
   if ( hFile == INVALID_HANDLE_VALUE )
   {
      // Display error
      switch ( ::GetLastError ( ) )
      {
         case ERROR_FILE_NOT_FOUND:
            // The specified COM-port does not exist
            return EPortNotAvailable;

         case ERROR_ACCESS_DENIED:
            // The specified COM-port is in use
            return EPortInUse;

         default:
            // Something else is wrong
            return EPortUnknownError;
      }
   }

   // Close handle
   ::CloseHandle ( hFile );

   // Port is available
   return EPortAvailable;
}

LONG SerialBus::Open ( LPCTSTR lpszDevice, DWORD dwInQueue, DWORD dwOutQueue, bool fOverlapped )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the port isn't already opened
   if ( m_hFile )
   {
      m_lLastError = ERROR_ALREADY_INITIALIZED;
      _RPTF0 ( _CRT_WARN, "SerialBus::Open - Port already opened\n" );
      return m_lLastError;
   }

   // Open the device
   m_hFile = ::CreateFile ( lpszDevice,
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            0,
                            OPEN_ALWAYS,
                            fOverlapped ? FILE_FLAG_OVERLAPPED : 0,
                            0 );
   if ( m_hFile == INVALID_HANDLE_VALUE )
   {
      // Reset file handle
      m_hFile = 0;

      // Display error
      m_lLastError = ::GetLastError ( );
      _RPTF0 ( _CRT_WARN, "SerialBus::Open - Unable to open port\n" );
      return m_lLastError;
   }

#ifndef SERIAL_NO_OVERLAPPED
   // We cannot have an event handle yet
   _ASSERTE ( m_hevtOverlapped == 0 );

   // Create the event handle for internal overlapped operations (manual reset)
   if ( fOverlapped )
   {
      m_hevtOverlapped = ::CreateEvent ( 0, true, false, 0 );
      if ( m_hevtOverlapped == 0 )
      {
         // Obtain the error information
         m_lLastError = ::GetLastError ( );
         _RPTF0 ( _CRT_WARN, "SerialBus::Open - Unable to create event\n" );

         // Close the port
         ::CloseHandle ( m_hFile );
         m_hFile = 0;

         // Return the error
         return m_lLastError;
      }
   }
#else

   // Overlapped flag shouldn't be specified
   _ASSERTE ( !fOverlapped );

#endif

   // Setup the COM-port
   if ( dwInQueue || dwOutQueue )
   {
      // Make sure the queue-sizes are reasonable sized. Win9X systems crash
      // if the input queue-size is zero. Both queues need to be at least
      // 16 bytes large.
      _ASSERTE ( dwInQueue >= 16 );
      _ASSERTE ( dwOutQueue >= 16 );

      if ( !::SetupComm ( m_hFile, dwInQueue, dwOutQueue ) )
      {
         // Display a warning
         long lLastError = ::GetLastError ( );
         _RPTF0 ( _CRT_WARN, "SerialBus::Open - Unable to setup the COM-port\n" );

         // Close the port
         Close ( );

         // Save last error from SetupComm
         m_lLastError = lLastError;
         return m_lLastError;
      }
   }

   // Setup the default communication mask
   SetMask ( );

   // Non-blocking reads is default
   SetupReadTimeouts ( EReadTimeoutNonblocking );

   // Setup the device for default settings
   COMMCONFIG commConfig = { 0 };
   DWORD dwSize = sizeof ( commConfig );
   commConfig.dwSize = dwSize;
   if ( ::GetDefaultCommConfig ( lpszDevice, &commConfig, &dwSize ) )
   {
      // Set the default communication configuration
      if ( !::SetCommConfig ( m_hFile, &commConfig, dwSize ) )
      {
         // Display a warning
         _RPTF0 ( _CRT_WARN, "SerialBus::Open - Unable to set default communication configuration.\n" );
      }
   }
   else
   {
      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::Open - Unable to obtain default communication configuration.\n" );
   }

   // Return successful
   return m_lLastError;
      }

LONG SerialBus::Close ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // If the device is already closed,
   // then we don't need to do anything.
   if ( m_hFile == 0 )
   {
      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::Close - Method called when device is not open\n" );
      return m_lLastError;
   }

#ifndef SERIAL_NO_OVERLAPPED
   // Free event handle
   if ( m_hevtOverlapped )
   {
      ::CloseHandle ( m_hevtOverlapped );
      m_hevtOverlapped = 0;
   }
#endif

   // Close COM port
   ::CloseHandle ( m_hFile );
   m_hFile = 0;

   // Return successful
   return m_lLastError;
}

LONG SerialBus::Setup ( EBaudrate eBaudrate, EDataBits eDataBits, EParity eParity, EStopBits eStopBits )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::Setup - Device is not opened\n" );
      return m_lLastError;
   }

   // Obtain the DCB structure for the device
   CDCB dcb;
   if ( !::GetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::Setup - Unable to obtain DCB information\n" );
      return m_lLastError;
   }

   // Set the new data
   dcb.BaudRate = DWORD ( eBaudrate );
   dcb.ByteSize = BYTE ( eDataBits );
   dcb.Parity = BYTE ( eParity );
   dcb.StopBits = BYTE ( eStopBits );

   // Determine if parity is used
   dcb.fParity = ( eParity != EParNone );

   // Set the new DCB structure
   if ( !::SetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::Setup - Unable to set DCB information\n" );
      return m_lLastError;
   }

   // Return successful
   return m_lLastError;
}

LONG SerialBus::SetEventChar ( BYTE bEventChar, bool fAdjustMask )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::SetEventChar - Device is not opened\n" );
      return m_lLastError;
   }

   // Obtain the DCB structure for the device
   CDCB dcb;
   if ( !::GetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::SetEventChar - Unable to obtain DCB information\n" );
      return m_lLastError;
   }

   // Set the new event character
   dcb.EvtChar = char ( bEventChar );

   // Adjust the event mask, to make sure the event will be received
   if ( fAdjustMask )
   {
      // Enable 'receive event character' event.  Note that this
      // will generate an EEventNone if there is an asynchronous
      // WaitCommEvent pending.
      SetMask ( GetEventMask ( ) | EEventRcvEv );
   }

   // Set the new DCB structure
   if ( !::SetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::SetEventChar - Unable to set DCB information\n" );
      return m_lLastError;
   }

   // Return successful
   return m_lLastError;
}

LONG SerialBus::SetMask ( DWORD dwEventMask )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::SetMask - Device is not opened\n" );
      return m_lLastError;
   }

   // Set the new mask. Note that this will generate an EEventNone
   // if there is an asynchronous WaitCommEvent pending.
   if ( !::SetCommMask ( m_hFile, dwEventMask ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::SetMask - Unable to set event mask\n" );
      return m_lLastError;
   }

   // Save event mask and return successful
   m_dwEventMask = dwEventMask;
   return m_lLastError;
}

LONG SerialBus::WaitEvent ( LPOVERLAPPED lpOverlapped, DWORD dwTimeout )
{
   // Check if time-outs are supported
   CheckRequirements ( lpOverlapped, dwTimeout );

   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::WaitEvent - Device is not opened\n" );
      return m_lLastError;
   }

#ifndef SERIAL_NO_OVERLAPPED

   // Check if an overlapped structure has been specified
   if ( !m_hevtOverlapped && ( lpOverlapped || ( dwTimeout != INFINITE ) ) )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_FUNCTION;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::WaitEvent - Overlapped I/O is disabled, specified parameters are illegal.\n" );
      return m_lLastError;
   }

   // Wait for the event to happen
   OVERLAPPED ovInternal;
   if ( !lpOverlapped && m_hevtOverlapped )
   {
      // Setup our own overlapped structure
      memset ( &ovInternal, 0, sizeof ( ovInternal ) );
      ovInternal.hEvent = m_hevtOverlapped;

      // Use our internal overlapped structure
      lpOverlapped = &ovInternal;
   }

   // Make sure the overlapped structure isn't busy
   _ASSERTE ( !m_hevtOverlapped || HasOverlappedIoCompleted ( lpOverlapped ) );

   // Wait for the COM event
   if ( !::WaitCommEvent ( m_hFile, LPDWORD ( &m_eEvent ), lpOverlapped ) )
   {
      // Set the internal error code
      long lLastError = ::GetLastError ( );

      // Overlapped operation in progress is not an actual error
      if ( lLastError != ERROR_IO_PENDING )
      {
         // Save the error
         m_lLastError = lLastError;

         // Issue an error and quit
         _RPTF0 ( _CRT_WARN, "SerialBus::WaitEvent - Unable to wait for COM event\n" );
         return m_lLastError;
      }

      // We need to block if the client didn't specify an overlapped structure
      if ( lpOverlapped == &ovInternal )
      {
         // Wait for the overlapped operation to complete
         switch ( ::WaitForSingleObject ( lpOverlapped->hEvent, dwTimeout ) )
         {
            case WAIT_OBJECT_0:
               // The overlapped operation has completed
               break;

            case WAIT_TIMEOUT:
               // Cancel the I/O operation
               CancelCommIo ( );

               // The operation timed out. Set the internal error code and quit
               m_lLastError = ERROR_TIMEOUT;
               return m_lLastError;

            default:
               // Set the internal error code
               m_lLastError = ::GetLastError ( );

               // Issue an error and quit
               _RPTF0 ( _CRT_WARN, "SerialBus::WaitEvent - Unable to wait until COM event has arrived\n" );
               return m_lLastError;
         }
      }
   }
   else
   {
      // The operation completed immediatly. Just to be sure
      // we'll set the overlapped structure's event handle.
      if ( lpOverlapped )
         ::SetEvent ( lpOverlapped->hEvent );
   }
#else

   // Wait for the COM event
   if ( !::WaitCommEvent ( m_hFile, LPDWORD ( &m_eEvent ), 0 ) )
   {
      // Set the internal error code
      m_lLastError = ::GetLastError ( );

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::WaitEvent - Unable to wait for COM event\n" );
      return m_lLastError;
   }

#endif

   // Return successfully
   return m_lLastError;
}


LONG SerialBus::SetupHandshaking ( EHandshake eHandshake )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::SetupHandshaking - Device is not opened\n" );
      return m_lLastError;
   }

   // Obtain the DCB structure for the device
   CDCB dcb;
   if ( !::GetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::SetupHandshaking - Unable to obtain DCB information\n" );
      return m_lLastError;
   }
   // Set the handshaking flags
   switch ( eHandshake )
   {
      case EHandshakeOff:
         dcb.fOutxCtsFlow = false;					// Disable CTS monitoring
         dcb.fOutxDsrFlow = false;					// Disable DSR monitoring
         dcb.fDtrControl = DTR_CONTROL_DISABLE;		// Disable DTR monitoring
         dcb.fOutX = false;							// Disable XON/XOFF for transmission
         dcb.fInX = false;							// Disable XON/XOFF for receiving
         dcb.fRtsControl = RTS_CONTROL_DISABLE;		// Disable RTS (Ready To Send)
         break;

      case EHandshakeHardware:
         dcb.fOutxCtsFlow = true;					// Enable CTS monitoring
         dcb.fOutxDsrFlow = true;					// Enable DSR monitoring
         dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;	// Enable DTR handshaking
         dcb.fOutX = false;							// Disable XON/XOFF for transmission
         dcb.fInX = false;							// Disable XON/XOFF for receiving
         dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;	// Enable RTS handshaking
         break;

      case EHandshakeSoftware:
         dcb.fOutxCtsFlow = false;					// Disable CTS (Clear To Send)
         dcb.fOutxDsrFlow = false;					// Disable DSR (Data Set Ready)
         dcb.fDtrControl = DTR_CONTROL_DISABLE;		// Disable DTR (Data Terminal Ready)
         dcb.fOutX = true;							// Enable XON/XOFF for transmission
         dcb.fInX = true;							// Enable XON/XOFF for receiving
         dcb.fRtsControl = RTS_CONTROL_DISABLE;		// Disable RTS (Ready To Send)
         break;

      default:
         // This shouldn't be possible
         _ASSERTE ( false );
         m_lLastError = E_INVALIDARG;
         return m_lLastError;
   }

   // Set the new DCB structure
   if ( !::SetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::SetupHandshaking - Unable to set DCB information\n" );
      return m_lLastError;
   }

   // Return successful
   return m_lLastError;
}

LONG SerialBus::SetupReadTimeouts ( EReadTimeout eReadTimeout )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::SetupReadTimeouts - Device is not opened\n" );
      return m_lLastError;
   }

   // Determine the time-outs
   COMMTIMEOUTS cto;
   if ( !::GetCommTimeouts ( m_hFile, &cto ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::SetupReadTimeouts - Unable to obtain timeout information\n" );
      return m_lLastError;
   }

   // Set the new timeouts
   switch ( eReadTimeout )
   {
      case EReadTimeoutBlocking:
         cto.ReadIntervalTimeout = 0;
         cto.ReadTotalTimeoutConstant = 0;
         cto.ReadTotalTimeoutMultiplier = 0;
         break;
      case EReadTimeoutNonblocking:
         cto.ReadIntervalTimeout = MAXDWORD;
         cto.ReadTotalTimeoutConstant = 0;
         cto.ReadTotalTimeoutMultiplier = 0;
         break;
      default:
         // This shouldn't be possible
         _ASSERTE ( false );
         m_lLastError = E_INVALIDARG;
         return m_lLastError;
   }

   // Set the new DCB structure
   if ( !::SetCommTimeouts ( m_hFile, &cto ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::SetupReadTimeouts - Unable to set timeout information\n" );
      return m_lLastError;
   }

   // Return successful
   return m_lLastError;
}

SerialBus::EBaudrate SerialBus::GetBaudrate ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::GetBaudrate - Device is not opened\n" );
      return EBaudUnknown;
   }

   // Obtain the DCB structure for the device
   CDCB dcb;
   if ( !::GetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetBaudrate - Unable to obtain DCB information\n" );
      return EBaudUnknown;
   }

   // Return the appropriate baudrate
   return EBaudrate ( dcb.BaudRate );
}

SerialBus::EDataBits SerialBus::GetDataBits ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::GetDataBits - Device is not opened\n" );
      return EDataUnknown;
   }

   // Obtain the DCB structure for the device
   CDCB dcb;
   if ( !::GetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetDataBits - Unable to obtain DCB information\n" );
      return EDataUnknown;
   }

   // Return the appropriate bytesize
   return EDataBits ( dcb.ByteSize );
}

SerialBus::EParity SerialBus::GetParity ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::GetParity - Device is not opened\n" );
      return EParUnknown;
   }

   // Obtain the DCB structure for the device
   CDCB dcb;
   if ( !::GetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetParity - Unable to obtain DCB information\n" );
      return EParUnknown;
   }

   // Check if parity is used
   if ( !dcb.fParity )
   {
      // No parity
      return EParNone;
   }

   // Return the appropriate parity setting
   return EParity ( dcb.Parity );
}

SerialBus::EStopBits SerialBus::GetStopBits ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::GetStopBits - Device is not opened\n" );
      return EStopUnknown;
   }

   // Obtain the DCB structure for the device
   CDCB dcb;
   if ( !::GetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetStopBits - Unable to obtain DCB information\n" );
      return EStopUnknown;
   }

   // Return the appropriate stopbits
   return EStopBits ( dcb.StopBits );
}

DWORD SerialBus::GetEventMask ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::GetEventMask - Device is not opened\n" );
      return 0;
   }

   // Return the event mask
   return m_dwEventMask;
}

BYTE SerialBus::GetEventChar ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::GetEventChar - Device is not opened\n" );
      return 0;
   }

   // Obtain the DCB structure for the device
   CDCB dcb;
   if ( !::GetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetEventChar - Unable to obtain DCB information\n" );
      return 0;
   }

   // Set the new event character
   return BYTE ( dcb.EvtChar );
}

SerialBus::EHandshake SerialBus::GetHandshaking ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::GetHandshaking - Device is not opened\n" );
      return EHandshakeUnknown;
   }

   // Obtain the DCB structure for the device
   CDCB dcb;
   if ( !::GetCommState ( m_hFile, &dcb ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetHandshaking - Unable to obtain DCB information\n" );
      return EHandshakeUnknown;
   }

   // Check if hardware handshaking is being used
   if ( ( dcb.fDtrControl == DTR_CONTROL_HANDSHAKE ) && ( dcb.fRtsControl == RTS_CONTROL_HANDSHAKE ) )
      return EHandshakeHardware;

   // Check if software handshaking is being used
   if ( dcb.fOutX && dcb.fInX )
      return EHandshakeSoftware;

   // No handshaking is being used
   return EHandshakeOff;
}

LONG SerialBus::Write ( const void* pData, size_t iLen, DWORD* pdwWritten, LPOVERLAPPED lpOverlapped, DWORD dwTimeout )
{
   // Check if time-outs are supported
   CheckRequirements ( lpOverlapped, dwTimeout );

   // Overlapped operation should specify the pdwWritten variable
   _ASSERTE ( !lpOverlapped || pdwWritten );

   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Use our own variable for read count
   DWORD dwWritten;
   if ( pdwWritten == 0 )
   {
      pdwWritten = &dwWritten;
   }

   // Reset the number of bytes written
   *pdwWritten = 0;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::Write - Device is not opened\n" );
      return m_lLastError;
   }

#ifndef SERIAL_NO_OVERLAPPED

   // Check if an overlapped structure has been specified
   if ( !m_hevtOverlapped && ( lpOverlapped || ( dwTimeout != INFINITE ) ) )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_FUNCTION;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::Write - Overlapped I/O is disabled, specified parameters are illegal.\n" );
      return m_lLastError;
   }

   // Wait for the event to happen
   OVERLAPPED ovInternal;
   if ( !lpOverlapped && m_hevtOverlapped )
   {
      // Setup our own overlapped structure
      memset ( &ovInternal, 0, sizeof ( ovInternal ) );
      ovInternal.hEvent = m_hevtOverlapped;

      // Use our internal overlapped structure
      lpOverlapped = &ovInternal;
   }

   // Make sure the overlapped structure isn't busy
   _ASSERTE ( !m_hevtOverlapped || HasOverlappedIoCompleted ( lpOverlapped ) );

   // Write the data
   if ( !::WriteFile ( m_hFile, pData, iLen, pdwWritten, lpOverlapped ) )
   {
      // Set the internal error code
      long lLastError = ::GetLastError ( );

      // Overlapped operation in progress is not an actual error
      if ( lLastError != ERROR_IO_PENDING )
      {
         // Save the error
         m_lLastError = lLastError;

         // Issue an error and quit
         _RPTF0 ( _CRT_WARN, "SerialBus::Write - Unable to write the data\n" );
         return m_lLastError;
      }

      // We need to block if the client didn't specify an overlapped structure
      if ( lpOverlapped == &ovInternal )
      {
         // Wait for the overlapped operation to complete
         switch ( ::WaitForSingleObject ( lpOverlapped->hEvent, dwTimeout ) )
         {
            case WAIT_OBJECT_0:
               // The overlapped operation has completed
               if ( !::GetOverlappedResult ( m_hFile, lpOverlapped, pdwWritten, FALSE ) )
               {
                  // Set the internal error code
                  m_lLastError = ::GetLastError ( );

                  _RPTF0 ( _CRT_WARN, "SerialBus::Write - Overlapped completed without result\n" );
                  return m_lLastError;
               }
               break;

            case WAIT_TIMEOUT:
               // Cancel the I/O operation
               CancelCommIo ( );

               // The operation timed out. Set the internal error code and quit
               m_lLastError = ERROR_TIMEOUT;
               return m_lLastError;

            default:
               // Set the internal error code
               m_lLastError = ::GetLastError ( );

               // Issue an error and quit
               _RPTF0 ( _CRT_WARN, "SerialBus::Write - Unable to wait until data has been sent\n" );
               return m_lLastError;
         }
      }
   }
   else
   {
      // The operation completed immediatly. Just to be sure
      // we'll set the overlapped structure's event handle.
      if ( lpOverlapped )
         ::SetEvent ( lpOverlapped->hEvent );
   }

#else

   // Write the data
   if ( !::WriteFile ( m_hFile, pData, iLen, pdwWritten, 0 ) )
   {
      // Set the internal error code
      m_lLastError = ::GetLastError ( );

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::Write - Unable to write the data\n" );
      return m_lLastError;
   }

#endif

   // Return successfully
   return m_lLastError;
}

LONG SerialBus::Write ( LPCSTR pString, DWORD* pdwWritten, LPOVERLAPPED lpOverlapped, DWORD dwTimeout )
{
   // Check if time-outs are supported
   CheckRequirements ( lpOverlapped, dwTimeout );

   // Determine the length of the string
   return Write ( pString, strlen ( pString ), pdwWritten, lpOverlapped, dwTimeout );
}

LONG SerialBus::Read ( void* pData, size_t iLen, DWORD* pdwRead, LPOVERLAPPED lpOverlapped, DWORD dwTimeout )
{
   // Check if time-outs are supported
   CheckRequirements ( lpOverlapped, dwTimeout );

   // Overlapped operation should specify the pdwRead variable
   _ASSERTE ( !lpOverlapped || pdwRead );

   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Use our own variable for read count
   DWORD dwRead;
   if ( pdwRead == 0 )
   {
      pdwRead = &dwRead;
   }

   // Reset the number of bytes read
   *pdwRead = 0;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::Read - Device is not opened\n" );
      return m_lLastError;
   }

#ifdef _DEBUG
   // The debug version fills the entire data structure with
   // 0xDC bytes, to catch buffer errors as soon as possible.
   memset ( pData, 0xDC, iLen );
#endif

#ifndef SERIAL_NO_OVERLAPPED

   // Check if an overlapped structure has been specified
   if ( !m_hevtOverlapped && ( lpOverlapped || ( dwTimeout != INFINITE ) ) )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_FUNCTION;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::Read - Overlapped I/O is disabled, specified parameters are illegal.\n" );
      return m_lLastError;
   }

   // Wait for the event to happen
   OVERLAPPED ovInternal;
   if ( lpOverlapped == 0 )
   {
      // Setup our own overlapped structure
      memset ( &ovInternal, 0, sizeof ( ovInternal ) );
      ovInternal.hEvent = m_hevtOverlapped;

      // Use our internal overlapped structure
      lpOverlapped = &ovInternal;
   }

   // Make sure the overlapped structure isn't busy
   _ASSERTE ( !m_hevtOverlapped || HasOverlappedIoCompleted ( lpOverlapped ) );

   // Read the data
   if ( !::ReadFile ( m_hFile, pData, iLen, pdwRead, lpOverlapped ) )
   {
      // Set the internal error code
      long lLastError = ::GetLastError ( );

      // Overlapped operation in progress is not an actual error
      if ( lLastError != ERROR_IO_PENDING )
      {
         // Save the error
         m_lLastError = lLastError;

         // Issue an error and quit
         _RPTF0 ( _CRT_WARN, "SerialBus::Read - Unable to read the data\n" );
         return m_lLastError;
      }

      // We need to block if the client didn't specify an overlapped structure
      if ( lpOverlapped == &ovInternal )
      {
         // Wait for the overlapped operation to complete
         switch ( ::WaitForSingleObject ( lpOverlapped->hEvent, dwTimeout ) )
         {
            case WAIT_OBJECT_0:
               // The overlapped operation has completed
               if ( !::GetOverlappedResult ( m_hFile, lpOverlapped, pdwRead, FALSE ) )
               {
                  // Set the internal error code
                  m_lLastError = ::GetLastError ( );

                  _RPTF0 ( _CRT_WARN, "SerialBus::Read - Overlapped completed without result\n" );
                  return m_lLastError;
               }
               break;

            case WAIT_TIMEOUT:
               // Cancel the I/O operation
               CancelCommIo ( );

               // The operation timed out. Set the internal error code and quit
               m_lLastError = ERROR_TIMEOUT;
               return m_lLastError;

            default:
               // Set the internal error code
               m_lLastError = ::GetLastError ( );

               // Issue an error and quit
               _RPTF0 ( _CRT_WARN, "SerialBus::Read - Unable to wait until data has been read\n" );
               return m_lLastError;
         }
      }
   }
   else
   {
      // The operation completed immediatly. Just to be sure
      // we'll set the overlapped structure's event handle.
      if ( lpOverlapped )
         ::SetEvent ( lpOverlapped->hEvent );
   }

#else

   // Read the data
   if ( !::ReadFile ( m_hFile, pData, iLen, pdwRead, 0 ) )
   {
      // Set the internal error code
      m_lLastError = ::GetLastError ( );

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::Read - Unable to read the data\n" );
      return m_lLastError;
   }

#endif

   // Return successfully
   return m_lLastError;
}

LONG SerialBus::Purge ( )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::Purge - Device is not opened\n" );
      return m_lLastError;
   }

   if ( !::PurgeComm ( m_hFile, PURGE_TXCLEAR | PURGE_RXCLEAR ) )
   {
      // Set the internal error code
      m_lLastError = ::GetLastError ( );
      _RPTF0 ( _CRT_WARN, "SerialBus::Purge - Overlapped completed without result\n" );
   }

   // Return successfully
   return m_lLastError;
}

LONG SerialBus::Break ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::Break - Device is not opened\n" );
      return m_lLastError;
   }

   // Set the RS-232 port in break mode for a little while
   ::SetCommBreak ( m_hFile );
   ::Sleep ( 100 );
   ::ClearCommBreak ( m_hFile );

   // Return successfully
   return m_lLastError;
}

SerialBus::EEvent SerialBus::GetEventType ( void )
{
#ifdef _DEBUG
   // Check if the event is within the mask
   if ( ( m_eEvent & m_dwEventMask ) == 0 )
      _RPTF2 ( _CRT_WARN, "SerialBus::GetEventType - Event %08Xh not within mask %08Xh.\n", m_eEvent, m_dwEventMask );
#endif

   // Obtain the event (mask unwanted events out)
   EEvent eEvent = EEvent ( m_eEvent & m_dwEventMask );

   // Reset internal event type
   m_eEvent = EEventNone;

   // Return the current cause
   return eEvent;
}

SerialBus::EError SerialBus::GetError ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Check if the device is open
   if ( m_hFile == 0 )
   {
      // Set the internal error code
      m_lLastError = ERROR_INVALID_HANDLE;

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::GetError - Device is not opened\n" );
      return EErrorUnknown;
   }

   // Obtain COM status
   DWORD dwErrors = 0;
   if ( !::ClearCommError ( m_hFile, &dwErrors, 0 ) )
   {
      // Set the internal error code
      m_lLastError = ::GetLastError ( );

      // Issue an error and quit
      _RPTF0 ( _CRT_WARN, "SerialBus::GetError - Unable to obtain COM status\n" );
      return EErrorUnknown;
   }

   // Return the error
   return EError ( dwErrors );
}

bool SerialBus::GetCTS ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Obtain the modem status
   DWORD dwModemStat = 0;
   if ( !::GetCommModemStatus ( m_hFile, &dwModemStat ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetCTS - Unable to obtain the modem status\n" );
      return false;
   }

   // Determine if CTS is on
   return ( dwModemStat & MS_CTS_ON ) != 0;
}

bool SerialBus::GetDSR ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Obtain the modem status
   DWORD dwModemStat = 0;
   if ( !::GetCommModemStatus ( m_hFile, &dwModemStat ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetDSR - Unable to obtain the modem status\n" );
      return false;
   }

   // Determine if DSR is on
   return ( dwModemStat & MS_DSR_ON ) != 0;
}

bool SerialBus::GetRing ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Obtain the modem status
   DWORD dwModemStat = 0;
   if ( !::GetCommModemStatus ( m_hFile, &dwModemStat ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetRing - Unable to obtain the modem status" );
      return false;
   }

   // Determine if Ring is on
   return ( dwModemStat & MS_RING_ON ) != 0;
}

bool SerialBus::GetRLSD ( void )
{
   // Reset error state
   m_lLastError = ERROR_SUCCESS;

   // Obtain the modem status
   DWORD dwModemStat = 0;
   if ( !::GetCommModemStatus ( m_hFile, &dwModemStat ) )
   {
      // Obtain the error code
      m_lLastError = ::GetLastError ( );

      // Display a warning
      _RPTF0 ( _CRT_WARN, "SerialBus::GetRLSD - Unable to obtain the modem status" );
      return false;
   }

   // Determine if RLSD is on
   return ( dwModemStat & MS_RLSD_ON ) != 0;
}
