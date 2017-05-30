/*
*  Simple function to print coupon
*
*  Build by wilson.souza
*  Default language used c++11
*
*  Copyright (c) 2016 PingoBox Software
*  Copyright (c) 2016 WR DevInfo
*
*  Created july 01 2016
*  Last updated
*  Version 0.1-beta
*/
#include <ThermalPrinter.h>

using namespace pthermal;
//-----------------------------------------------------------------------------------------------//
bool PTHERMAL_API __stdcall PrintCoupon ( std::wstring const & szXMLFileName,
                                          pthermal::CouponType const & uType)
{
   try
   {
      LPThermalPrinter p = LPThermalPrinter ( new ThermalPrinter ( ) );

      if ( uType == CouponType::coupon )
      {
         return p->Build ( szXMLFileName, pthermal::printer::mode::WindowsDriver );
      }
      else
      {
         return p->Build ( szXMLFileName, pthermal::printer::mode::WindowsDriver, uType );
      }
   }
   catch ( std::exception const & e )
   {
      //dispatch internal exception
      throw e;
   }
   return false;
}
//-----------------------------------------------------------------------------------------------//
