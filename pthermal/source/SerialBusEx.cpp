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
#include <crtdbg.h>
#include <SerialBusEx.h>


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
SerialBusEx::SerialBusEx()
	: m_fStopping(false)
	, m_hThread(0)
#ifndef SERIAL_NO_OVERLAPPED
	, m_hevtOverlappedWorkerThread(0)
#endif
{
}

SerialBusEx::~SerialBusEx()
{
	// Check if the thread handle is still there. If so, then we
	// didn't close the serial port. We cannot depend on the
	// SerialBus destructor, because if it calls Close then it
	// won't call our overridden Close.
	if (m_hThread)
	{
		// Display a warning
		_RPTF0(_CRT_WARN, "SerialBusEx::~SerialBusEx - Serial port not closed\n");

		// Close implicitly
		Close();
	}
}

LONG SerialBusEx::Open (LPCTSTR lpszDevice, DWORD dwInQueue, DWORD dwOutQueue, bool fStartListener)
{
	// Call the base class first
	long lLastError = SerialBus::Open(lpszDevice,dwInQueue,dwOutQueue,SERIAL_DEFAULT_OVERLAPPED);
	if (lLastError != ERROR_SUCCESS)
		return lLastError;

#ifndef SERIAL_NO_OVERLAPPED
	// Create an event that is used for the workerthread. The globally
	// used event is used by the main-thread and cannot be reused
	// for this thread.
	m_hevtOverlappedWorkerThread = ::CreateEvent(0,true,false,0);
	if (m_hevtOverlappedWorkerThread == 0)
	{
		// Obtain the error information
		m_lLastError = ::GetLastError();
		_RPTF0(_CRT_WARN,"SerialBusEx::Open - Unable to create event\n");

		// Close the serial port again
		SerialBus::Close();

		// Return the error
		return m_lLastError;
	}
#endif


	// Start the listener thread (only on request)
	if (fStartListener)
	{
		lLastError = StartListener();
		if (lLastError != ERROR_SUCCESS)
			return lLastError;
	}

	// Return the error
	return m_lLastError;
}

LONG SerialBusEx::Close (void)
{
	// Stop listener thread (wait until it ends)
	StopListener(INFINITE);

#ifndef SERIAL_NO_OVERLAPPED
	// Free event handle
	if (m_hevtOverlappedWorkerThread)
	{
		::CloseHandle(m_hevtOverlappedWorkerThread);
		m_hevtOverlappedWorkerThread = 0;

	}
#endif

	// Call the base class
	return SerialBus::Close();
}

LONG SerialBusEx::StartListener (void)
{
	// Check if the watcher thread was already running
	if (m_hThread == 0)
	{
		// Make sure the thread has stopped
		_ASSERTE(!m_fStopping);

		// Start the watcher thread
		DWORD dwThreadId = 0;
		m_hThread = ::CreateThread(0,0,ThreadProc,LPVOID(this),0,&dwThreadId);
		if (m_hThread == 0)
		{
			// Display a warning
			_RPTF0(_CRT_WARN, "SerialBusEx::StartListener - Unable to start COMM watcher thread\n");

			// Set the error code and exit
			m_lLastError = ::GetLastError();
			return m_lLastError;
		}
	}

	// Return the error
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

LONG SerialBusEx::StopListener (DWORD dwTimeout)
{
	// Check if the thread is running
	if (m_hThread)
	{
		// Set the flag that the thread must be stopped
		m_fStopping = true;

		// Cancel the pending WaitEvent, but we won't do this using
		// CancelIo. This would break Win95 compatibility and some
		// USB serial dongles cannot handle CancelIo correctly. By
		// setting the event mask again, the call will also be
		// completed before the thread exits.
		SetMask(GetEventMask());
		
		// Wait until the watcher thread has stopped
		::WaitForSingleObject(m_hThread,dwTimeout);

		// The thread has stopped
		m_fStopping = false;

		// Close handle to the thread
		::CloseHandle(m_hThread);
		m_hThread = 0;
	}

	// Return the error
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

DWORD WINAPI SerialBusEx::ThreadProc (LPVOID lpArg)
{
	// Route the method to the actual object
	SerialBusEx* pThis = reinterpret_cast<SerialBusEx*>(lpArg);
	return pThis->ThreadProc();
}

DWORD SerialBusEx::ThreadProc (void)
{
	// Use overlapped structure
	LPOVERLAPPED lpOverlapped = 0;

	// Keep looping
	do
	{
#ifndef SERIAL_NO_OVERLAPPED
		// Reset the event handle
		::ResetEvent(m_hevtOverlappedWorkerThread);

		// Initialize the overlapped structure
		OVERLAPPED ovInternal = {0};
		ovInternal.hEvent = m_hevtOverlappedWorkerThread;

		// Start the WaitEvent (use our own overlapped structure)
		if (WaitEvent(&ovInternal) != ERROR_SUCCESS)
			return m_lLastError;

		// Wait for the overlapped operation to complete
		if (::WaitForSingleObject(m_hevtOverlappedWorkerThread,INFINITE) != WAIT_OBJECT_0)
		{
			// Set the internal error code
			m_lLastError = ::GetLastError();

			// Issue an error and quit
			_RPTF0(_CRT_WARN,"SerialBusEx::ThreadProc - Unable to wait until COM event has arrived\n");
			return m_lLastError;
		}
#else
		// Start the WaitEvent (don't need to specify an overlapped structure)
		if (WaitEvent() != ERROR_SUCCESS)
			return m_lLastError;
#endif

		// Wait until one of the events happens
		if (!m_fStopping)
		{
			// Determine the event
			EEvent eEvent = GetEventType();

			// Obtain the error status during this event
			DWORD dwErrors = 0;
			if (!::ClearCommError(m_hFile,&dwErrors,0))
			{
				// Set the internal error code
				m_lLastError = ::GetLastError();

				// Issue an error and quit
				_RPTF0(_CRT_WARN, "SerialBusEx::ThreadProc - Unable to obtain COM status\n");
			}

			// Convert the error
			EError eError = EError(dwErrors);

			// There was a COMM event, which needs handling. We'll call the
			// event handler. We can receive a "zero" event, when the
			// mask or event character has been set. We won't pass this
			// down to the window.
			if (eEvent)
				OnEvent(eEvent,eError);
		}
	}
	while (!m_fStopping);

	// Bye bye
	return 0;
}