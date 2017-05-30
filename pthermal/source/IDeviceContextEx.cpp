/*
*  Layer to Thermal Printer via WinSpool
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created jun 15 2016
*  Last updated
*  Version 0.1-beta
*/
#include <IDeviceContextEx.hpp>
#include <GenerateTicketDataEx.hpp>
#include <escpos.hpp>
#include <escbema.hpp>
#include <IWcharToChar.h>

using namespace device;
//--------------------------------------------------------------------------------------------//
IDeviceContextEx::IDeviceContextEx ( pthermal::LPIUniversalSerialBusData const & pBus ) :
IDeviceContext ( pBus )
{

}
//--------------------------------------------------------------------------------------------//
bool IDeviceContextEx::Create ( pthermal::LPJsonData const & pData )
{
   IDeviceContext::Create ( NULL );
   m_Data->szDocumentName = pData->m_customer.name;
   m_json = pData;
   return true;
}
//--------------------------------------------------------------------------------------------//
bool IDeviceContextEx::PreparePage ( )
{
   IDeviceContext::PreparePage ( );
   pthermal::LPGenerateTicketDataEx q = 
      pthermal::LPGenerateTicketDataEx( new pthermal::GenerateTicketDataEx ( m_json, m_pBus ) );
   
   SelectObject ( m_thermal->hDC, m_thermal->Fonts->at ( 0 ) );
   Update ( );
   q->SetPaperWidth ( m_thermal->nColumns );
   m_queue = q->Build ( );
   return true;
}
//--------------------------------------------------------------------------------------------//
