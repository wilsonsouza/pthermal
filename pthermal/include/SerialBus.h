/*
*  Layer to USBCOM Control
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

#include <pthermal.hpp>
#include <ISerialBus.h>
//////////////////////////////////////////////////////////////////////
// The SERIAL_DEFAULT_OVERLAPPED defines if the default open mode uses
// overlapped I/O. When overlapped I/O is available (normal Win32
// platforms) it uses overlapped I/O. Windows CE doesn't allow the use
// of overlapped I/O, so it is disabled there by default.

#ifndef SERIAL_DEFAULT_OVERLAPPED
#ifndef SERIAL_NO_OVERLAPPED
#define SERIAL_DEFAULT_OVERLAPPED	true
#else
#define SERIAL_DEFAULT_OVERLAPPED	false
#endif
#endif


//////////////////////////////////////////////////////////////////////
//
// SerialBus - Win32 wrapper for serial communications
//
// Serial communication often causes a lot of problems. This class
// tries to supply an easy to use interface to deal with serial
// devices.
//
// The class is actually pretty ease to use. You only need to open
// the COM-port, where you need to specify the basic serial
// communication parameters. You can also choose to setup handshaking
// and read timeout behaviour.
//
// The following serial classes are available:
//
// ISerialBus     - Abstract Serial commnunication support.
// SerialBus      - Serial communication support.
// SerialBusEx    - Serial communication with listener thread for events
// SerialBusSync  - Serial communication with synchronized event handler
// SerialBusWnd   - Asynchronous serial support, which uses the Win32
//                message queue for event notification.
// SerialBusMFC   - Preferred class to use in MFC-based GUI windows.
// 
//
// Pros:
// -----
//	- Easy to use (hides a lot of nasty Win32 stuff)
//	- Fully ANSI and Unicode aware
//
// Cons:
// -----
//  - Little less flexibility then native Win32 API, however you can
//    use this API at the same time for features which are missing
//    from this class.
//  - Incompatible with Windows 95 or Windows NT v3.51 (or earlier),
//    because CancelIo isn't support on these platforms. Define the
//	  SERIAL_NO_CANCELIO macro for support of these platforms as
//	  well. When this macro is defined, then only time-out values of
//	  0 or INFINITE are valid.
namespace usbcom
{
   class PTHERMAL_API SerialBus : public ISerialBus
   {
      // Construction
   public:
      SerialBus();
      virtual ~SerialBus();
      //Enumeration
      virtual void EnumerateWin32Ports ( );
      // Operations
   public:
      // Check if particular COM-port is available (static method).
      static EPort CheckPort(LPCTSTR lpszDevice);

      // Open the serial communications for a particular COM port. You
      // need to use the full devicename (i.e. "COM1") to open the port.
      // It's possible to specify the size of the input/output queues.
      virtual LONG Open(LPCTSTR lpszDevice, DWORD dwInQueue = 0, DWORD dwOutQueue = 0, bool fOverlapped = SERIAL_DEFAULT_OVERLAPPED);

      // Close the serial port.
      virtual LONG Close(void);

      // Setup the communication settings such as baudrate, databits,
      // parity and stopbits. The default settings are applied when the
      // device has been opened. Call this function if these settings do
      // not apply for your application. If you prefer to use integers
      // instead of the enumerated types then just cast the integer to
      // the required type. So the following two initializations are
      // equivalent:
      //
      //   Setup(EBaud9600,EData8,EParNone,EStop1)
      //
      // or
      //
      //   Setup(EBaudrate(9600),EDataBits(8),EParity(NOPARITY),EStopBits(ONESTOPBIT))
      //
      // In the latter case, the types are not validated. So make sure
      // that you specify the appropriate values.
      virtual LONG Setup(EBaudrate eBaudrate = EBaud9600,
         EDataBits eDataBits = EData8,
         EParity   eParity = EParNone,
         EStopBits eStopBits = EStop1);

      // Set/clear the event character. When this byte is being received
      // on the serial port then the EEventRcvEv event is signalled,
      // when the mask has been set appropriately. If the fAdjustMask flag
      // has been set, then the event mask is automatically adjusted.
      virtual LONG SetEventChar(BYTE bEventChar, bool fAdjustMask = true);

      // Set the event mask, which indicates what events should be
      // monitored. The WaitEvent method can only monitor events that
      // have been enabled. The default setting only monitors the
      // error events and data events. An application may choose to
      // monitor CTS. DSR, RLSD, etc as well.
      virtual LONG SetMask(DWORD dwMask = EEventBreak | EEventError | EEventRecv);

      // The WaitEvent method waits for one of the events that are
      // enabled (see SetMask).
      virtual LONG WaitEvent(LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);

      // Setup the handshaking protocol. There are three forms of
      // handshaking:
      //
      // 1) No handshaking, so data is always send even if the receiver
      //    cannot handle the data anymore. This can lead to data loss,
      //    when the sender is able to transmit data faster then the
      //    receiver can handle.
      // 2) Hardware handshaking, where the RTS/CTS lines are used to
      //    indicate if data can be sent. This mode requires that both
      //    ports and the cable support hardware handshaking. Hardware
      //    handshaking is the most reliable and efficient form of
      //    handshaking available, but is hardware dependant.
      // 3) Software handshaking, where the XON/XOFF characters are used
      //    to throttle the data. A major drawback of this method is that
      //    these characters cannot be used for data anymore.
      virtual LONG SetupHandshaking(EHandshake eHandshake);

      // Read operations can be blocking or non-blocking. You can use
      // this method to setup wether to use blocking or non-blocking
      // reads. Non-blocking reads is the default, which is required
      // for most applications.
      //
      // 1) Blocking reads, which will cause the 'Read' method to block
      //    until the requested number of bytes have been read. This is
      //    useful if you know how many data you will receive.
      // 2) Non-blocking reads, which will read as many bytes into your
      //    buffer and returns almost immediately. This is often the
      //    preferred setting.
      virtual LONG SetupReadTimeouts(EReadTimeout eReadTimeout);

      // Obtain communication settings
      virtual EBaudrate  GetBaudrate(void);
      virtual EDataBits  GetDataBits(void);
      virtual EParity    GetParity(void);
      virtual EStopBits  GetStopBits(void);
      virtual EHandshake GetHandshaking(void);
      virtual DWORD      GetEventMask(void);
      virtual BYTE       GetEventChar(void);

      // Write data to the serial port. Note that we are only able to
      // send ANSI strings, because it probably doesn't make sense to
      // transmit Unicode strings to an application.
      virtual LONG Write(const void* pData, size_t iLen, DWORD* pdwWritten = 0, LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);
      virtual LONG Write(LPCSTR pString, DWORD* pdwWritten = 0, LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);

      // Read data from the serial port. Refer to the description of
      // the 'SetupReadTimeouts' for an explanation about (non) blocking
      // reads and how to use this.
      virtual LONG Read(void* pData, size_t iLen, DWORD* pdwRead = 0, LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);

      // Send a break
      virtual LONG Break(void);

      // Determine what caused the event to trigger
      virtual EEvent GetEventType(void);

      // Obtain the error
      virtual EError GetError(void);

      // Obtain the COMM and event handle
      virtual HANDLE GetCommHandle(void)
      {
         return m_hFile;
      }

      // Check if com-port is opened
      virtual bool IsOpen(void) const
      {
         return (m_hFile != 0);
      }

      // Obtain last error status
      virtual LONG GetLastError(void) const
      {
         return m_lLastError;
      }

      // Obtain CTS/DSR/RING/RLSD settings
      virtual bool GetCTS(void);
      virtual bool GetDSR(void);
      virtual bool GetRing(void);
      virtual bool GetRLSD(void);

      // Purge all buffers
      virtual LONG Purge(void);
   protected:
      // Check the requirements
      virtual void CheckRequirements(LPOVERLAPPED lpOverlapped, DWORD dwTimeout) const;

      // CancelIo wrapper (for Win95 compatibility)
      virtual BOOL CancelCommIo(void);
   };
   //
   typedef std::shared_ptr<SerialBus> LPSerialBus;
};