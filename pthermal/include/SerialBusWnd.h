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
#include <pthermal.hpp>
#include <SerialBusEx.h>

//////////////////////////////////////////////////////////////////////
//
// SerialBusWnd - Win32 message-based wrapper for serial communications
//
// A lot of MS-Windows GUI based programs use a central message
// loop, so the application cannot block to wait for objects. This
// make serial communication difficult, because it isn't event
// driven using a message queue. This class makes the SerialBus based
// classes suitable for use with such a messagequeue. Whenever
// an event occurs on the serial port, a user-defined message will
// be sent to a user-defined window. It can then use the standard
// message dispatching to handle the event.
//
// Pros:
// -----
//	- Easy to use
//	- Fully ANSI and Unicode aware
//  - Integrates easily in GUI applications and is intuitive to
//    use for GUI application programmers
//
// Cons:
// -----
//  - Uses a thread for each COM-port, which has been opened.
//  - More overhead, due to thread switching and message queues.
//  - Requires a window, but that's probably why you're using
//    this class.
namespace usbcom
{
   class SerialBusWnd : public SerialBusEx
   {
      // Construction
   public:
      SerialBusWnd();
      virtual ~SerialBusWnd();

      // Operations
   public:
      // Open the serial communications for a particular COM port. You
      // need to use the full devicename (i.e. "COM1") to open the port.
      virtual LONG Open(LPCTSTR lpszDevice, HWND hwndDest, UINT nComMsg = WM_NULL, LPARAM lParam = 0, DWORD dwInQueue = 0, DWORD dwOutQueue = 0);

      // Close the serial port.
      virtual LONG Close(void);

   protected:
      // Event handler that is called when 
      virtual void OnEvent(EEvent eEvent, EError eError);

   public:
      // Default Serial notification message
      static const UINT mg_nDefaultComMsg;

   protected:
      // Internal attributes
      HWND	m_hwndDest;
      UINT	m_nComMsg;
      LONG	m_lParam;
   };
};