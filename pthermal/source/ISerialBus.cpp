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
#include <ISerialBus.h>
using namespace usbcom;

ISerialBus::ISerialBus ( ) : m_lLastError ( ERROR_SUCCESS )
, m_hFile ( 0 )
, m_eEvent ( EEventNone )
, m_dwEventMask ( 0 )
#ifndef SERIAL_NO_OVERLAPPED
, m_hevtOverlapped ( 0 )
#endif
{
}
