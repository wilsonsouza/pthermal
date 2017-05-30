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
#include <SerialBusWnd.h>
#include <crtdbg.h>

//////////////////////////////////////////////////////////////////////
// Disable warning C4127: conditional expression is constant, which
// is generated when using the _RPTF and _ASSERTE macros.

#pragma warning(disable: 4127)


//////////////////////////////////////////////////////////////////////
// Enable debug memory manager

#ifdef _DEBUG

#ifdef THIS_FILE
#undef THIS_FILE
#endif

static const char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW

#endif


//////////////////////////////////////////////////////////////////////
// Code
using namespace usbcom;
// Register the standard SerialBusWnd COM message
const UINT SerialBusWnd::mg_nDefaultComMsg = ::RegisterWindowMessage(L"SerialBusWnd_DefaultComMsg");


SerialBusWnd::SerialBusWnd()
	: m_hwndDest(0)
	, m_nComMsg(WM_NULL)
	, m_lParam(0)
{
}

SerialBusWnd::~SerialBusWnd()
{
	// Check if the thread handle is still there. If so, then we
	// didn't close the serial port. We cannot depend on the
	// SerialBus destructor, because if it calls Close then it
	// won't call our overridden Close.
	if (m_hThread)
	{
		// Display a warning
		_RPTF0(_CRT_WARN, "SerialBusWnd::~SerialBusWnd - Serial port not closed\n");

		// Close implicitly
		Close();
	}
}

LONG SerialBusWnd::Open (LPCTSTR lpszDevice, HWND hwndDest, UINT nComMsg, LPARAM lParam, DWORD dwInQueue, DWORD dwOutQueue)
{
	// Call the base class first
	long lLastError = SerialBusEx::Open(lpszDevice,dwInQueue,dwOutQueue);
	if (lLastError != ERROR_SUCCESS)
		return lLastError;

	// Save the window handle, notification message and user message
	m_hwndDest = hwndDest;
	m_nComMsg  = nComMsg?nComMsg:mg_nDefaultComMsg;
	m_lParam   = lParam;

	// Start the listener thread
	lLastError = StartListener();
	if (lLastError != ERROR_SUCCESS)
	{
		// Close the serial port
		Close();

		// Return the error-code
		m_lLastError = lLastError;
		return m_lLastError;
	}

	// Return the error
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

LONG SerialBusWnd::Close (void)
{
	// Reset all members
	m_hwndDest   = 0;
	m_nComMsg    = WM_NULL;

	// Call the base class
	return SerialBusEx::Close();
}

void SerialBusWnd::OnEvent (EEvent eEvent, EError eError)
{
	// Post message to the client window
	::PostMessage(m_hwndDest,m_nComMsg,MAKEWPARAM(eEvent,eError),LPARAM(m_lParam));
}
