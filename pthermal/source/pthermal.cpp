/*
*  Layer to Thermal Printer Controller
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 2 2016
*  Last updated
*  Version 0.1-beta
*
*  Description:
*  The following ifdef block is the standard way of creating macros which make exporting 
*  from a DLL simpler. All files within this DLL are compiled with the PTHERMAL_EXPORTS
*  symbol defined on the command line. This symbol should not be defined on any project
*  that uses this DLL. This way any other project whose source files include this file see 
*  PTHERMAL_API functions as being imported from a DLL, whereas this DLL sees symbols
*  defined with this macro as being exported.
*/
#if defined(_DEBUG)
#pragma comment(lib, "QRGenerator.lib")
#pragma comment(lib, "libusb-1.0.lib")
#else
#pragma comment(lib, "QRGenerator.lib")
#pragma comment(lib, "libusb-1.0.lib")
#endif
//
#pragma comment(lib, "Setupapi.lib")

#include <stdafx.h>
#include <pthermal.hpp>
using namespace pthermal;

