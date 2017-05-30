/*
*  Layer to Thermal Printer
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
#include <pthermal.hpp>

DWORD pthermal::IThermalPrinterDefs::m_ulDriverMode = pthermal::printer::mode::VirtualUniversalSerialBus;
//-----------------------------------------------------------------------------------------------//
DWORD const pthermal::IThermalPrinterDefs::GetDriverMode ( )
{
   return m_ulDriverMode;
}
//-----------------------------------------------------------------------------------------------//
void const pthermal::IThermalPrinterDefs::SetDriverMode ( DWORD dwDriverMode )
{
   m_ulDriverMode = dwDriverMode;
}
//-----------------------------------------------------------------------------------------------//

